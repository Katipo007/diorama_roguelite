#pragma once

#include "InputButtonDevice.hpp"

namespace Input
{
	enum class TextControlCode
	{
		Enter,
		Delete,
		Backspace,
		CaretLeft,
		CaretRight,
		CaretUp,
		CaretDown,
		PageUp,
		PageDown,
		Home,
		End,
		Tab,
		Copy,
		Paste,
		Cut,
		Undo,
		Redo,
		SelectAll,
		SelectLeft,
		SelectRight,
		SelectUp,
		SelectDown
	};

	class KeyboardInput
		: public InputButtonDevice
	{
	public:
		KeyboardInput( size_t button_count );
		virtual ~KeyboardInput();

	protected:
		void OnButtonPressed( ButtonCode_T scancode ) override;
		void OnButtonReleased( ButtonCode_T scancode ) override;

		virtual void OnTextControlCodeGenerated( TextControlCode control_code );
	};
}
