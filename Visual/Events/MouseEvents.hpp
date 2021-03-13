#pragma once

#include "Event.hpp"
#include "Visual/Input/MouseCodes.hpp"

namespace Visual::Device::MouseEvents
{
	class Moved
		: public CompileTimeEvent< Moved >
	{
	public:
		Moved( const float x, const float y )
			: x( x )
			, y( y )
		{}
		~Moved() override = default;

		const float& GetX() const { return x; }
		const float& GetY() const { return y; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << x << ", " << y;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::MouseMoved; }
		static bitmask<EventCategory> GetStaticCategoryFlags() { return EventCategory::Mouse | EventCategory::Input; }

	private:
		float x;
		float y;
	};

	class Scrolled
		: public CompileTimeEvent< Scrolled >
	{
	public:
		Scrolled( const float x_offset, const float y_offset )
			: x_offset( x_offset )
			, y_offset( y_offset )
		{}
		~Scrolled() override = default;

		const float& GetXOffset() const { return x_offset; }
		const float& GetYOffset() const { return y_offset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << x_offset << ", " << y_offset;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::MouseScrolled; }
		static bitmask<EventCategory> GetStaticCategoryFlags() { return EventCategory::Mouse | EventCategory::Input; }

	private:
		float x_offset;
		float y_offset;
	};

	template<typename DERIVED>
	class BaseMouseButtonEvent
		: public CompileTimeEvent< DERIVED >
	{
	public:
		~BaseMouseButtonEvent() override = default;

		const ::Input::MouseCode& GetMouseButton() const { return button; }

		static bitmask<EventCategory> GetStaticCategoryFlags() { return EventCategory::Mouse | EventCategory::Input; }

	protected:
		BaseMouseButtonEvent( const ::Input::MouseCode button )
			: button( button ) {}

		::Input::MouseCode button;
	};

	class ButtonPressed
		: public BaseMouseButtonEvent<ButtonPressed>
	{
	public:
		ButtonPressed( const ::Input::MouseCode button )
			: BaseMouseButtonEvent( button ) {}
		~ButtonPressed() override = default;

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << button;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::MouseButtonPressed; }
	};

	class ButtonReleased
		: public BaseMouseButtonEvent<ButtonReleased>
	{
	public:
		ButtonReleased( const ::Input::MouseCode button )
			: BaseMouseButtonEvent( button ) {}
		~ButtonReleased() override = default;

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << button;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::MouseButtonReleased; }
	};
}
