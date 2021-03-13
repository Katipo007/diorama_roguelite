#pragma once

#include <functional>
#include <sstream>

#include "Common/Core/Base.hpp"
#include "Common/Vendor/enumclassbitset/EnumClassBitset.hpp"

namespace Visual::Device
{

	enum class EventType : uint16_t
	{
		None = 0,

		WindowClose,
		WindowResize,
		WindowFocus,
		WindowLostFocus,
		WindowMoved,

		ApplicationTick,
		ApplicationUpdate,
		ApplicationRender,

		KeyPressed,
		KeyReleased,
		KeyTyped,

		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled,
	};

	enum class EventCategory : uint16_t
	{
		None = 0,

		Application = BIT( 0 ),
		Input = BIT( 1 ),
		Keyboard = BIT( 2 ),
		Mouse = BIT( 3 ),
		MouseButton = BIT( 4 ),
	};

	class Event
	{
	public:
		virtual ~Event() = default;

		bool handled = false;

		virtual EventType GetEventType() const = 0;
		virtual std::string GetName() const = 0;
		virtual bitmask<EventCategory> GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory( EventCategory category )
		{
			return (GetCategoryFlags().value & (uint16_t)category);
		}
	};

	template<typename DERIVED>
	class CompileTimeEvent
		: public Event
	{
	public:
		virtual ~CompileTimeEvent() override = default;

		virtual EventType GetEventType() const override { return DERIVED::GetStaticType(); }
		virtual std::string GetName() const override { return std::to_string( static_cast<unsigned long>(DERIVED::GetStaticType()) ); }
		virtual bitmask<EventCategory> GetCategoryFlags() const override { return DERIVED::GetStaticCategoryFlags(); }
	};

	class EventDispatcher
	{
	public:
		EventDispatcher( Event& event_ )
			: event( event_ )
		{}

		template<typename T, typename F>
		bool Dispatch( const F& func )
		{
			static_assert(std::is_base_of<Event, T>::value, "Provided type does not derive from Event");
			static_assert(std::is_invocable<F, T&>::value, "Provided handler doesn't accept the correct event type");
			static_assert(std::is_same< bool, std::invoke_result<F, T&>::type >::value, "Provided event handler must return a boolean stating whether the event was handled");

			if ((this->event).GetEventType() == T::GetStaticType())
			{
				this->event.handled |= func( static_cast<T&>(this->event) );
				return true;
			}
			return false;
		}

	private:
		Event& event;
	};

	inline std::ostream& operator<<( std::ostream& os, const Event& e )
	{
		return os << e.ToString();
	}
}

/// ENABLE_ENUM_CLASS_BITMASK must be performed outside of any namespace
ENABLE_ENUM_CLASS_BITMASK( Visual::Device::EventCategory );
