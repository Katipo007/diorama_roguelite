#pragma once

#include "Event.hpp"
#include "Visual/Input/Keycodes.hpp"

namespace Visual::Device::KeyboardEvents
{
	template<typename DERIVED>
	class BaseKeyEvent
		: public CompileTimeEvent< DERIVED >
	{
	public:
		~BaseKeyEvent() override = default;

		const ::Input::KeyCode& GetKeyCode() const { return keycode; }

		static bitmask<EventCategory> GetStaticCategoryFlags() { return EventCategory::Keyboard | EventCategory::Keyboard; }

	protected:
		BaseKeyEvent( const ::Input::KeyCode keycode )
			: keycode( keycode ) {}

		::Input::KeyCode keycode;
	};

	class KeyPressed
		: public BaseKeyEvent< KeyPressed >
	{
	public:
		KeyPressed( const ::Input::KeyCode keycode, const uint16_t repeat_count )
			: BaseKeyEvent( keycode ), repeat_count( repeat_count ) {}
		~KeyPressed() override = default;

		const uint16_t& GetRepeatCount() const { return repeat_count; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << keycode << " (" << repeat_count << " repeats)";
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::KeyPressed; }

	private:
		uint16_t repeat_count;

	};

	class KeyReleased
		: public BaseKeyEvent< KeyReleased >
	{
	public:
		KeyReleased( const ::Input::KeyCode keycode )
			: BaseKeyEvent( keycode ) {}
		~KeyReleased() override = default;

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << keycode;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::KeyReleased; }
	};

	class KeyTypedEvent
		: public BaseKeyEvent< KeyTypedEvent >
	{
	public:
		KeyTypedEvent( const ::Input::KeyCode keycode )
			: BaseKeyEvent( keycode ) {}
		~KeyTypedEvent() override = default;

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << keycode;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::KeyTyped; }
	};
}
