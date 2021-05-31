#include "KeyboardInput.hpp"

#include <cstdio>

#include "Common/Utility/ConstExprContainers.hpp"
#include "Keycodes.hpp"

namespace Input
{
	KeyboardInput::KeyboardInput( size_t button_count )
		: InputButtonDevice( button_count )
	{
	}

	KeyboardInput::~KeyboardInput()
	{
	}

	void KeyboardInput::OnButtonPressed( ButtonCode_T scancode )
	{
		const bool is_shift_down = IsButtonDown( Keys::LShift ) || IsButtonDown( Keys::RShift );
		const bool is_ctrl_down = IsButtonDown( Keys::LCtrl ) || IsButtonDown( Keys::RCtrl );

		std::optional<TextControlCode> control_code = std::nullopt;

		if (!is_shift_down && !is_ctrl_down)
		{
			static constexpr frozen::unordered_map<ButtonCode_T, TextControlCode, 13> mappings{
				{ Keys::Backspace,		TextControlCode::Backspace },
				{ Keys::Delete,			TextControlCode::Delete },
				{ Keys::Enter,			TextControlCode::Enter },
				{ Keys::NumPad_Enter,	TextControlCode::Enter },
				{ Keys::Tab,			TextControlCode::Tab },
				{ Keys::Left,			TextControlCode::CaretLeft },
				{ Keys::Right,			TextControlCode::CaretRight },
				{ Keys::Up,				TextControlCode::CaretUp },
				{ Keys::Down,			TextControlCode::CaretDown },
				{ Keys::PageUp,			TextControlCode::PageUp },
				{ Keys::PageDown,		TextControlCode::PageDown },
				{ Keys::Home,			TextControlCode::Home },
				{ Keys::End,			TextControlCode::End },
			};

			if (mappings.count( scancode ))
				control_code = mappings.at( scancode );
		}

		if (!is_shift_down && is_ctrl_down)
		{
			static constexpr frozen::unordered_map<ButtonCode_T, TextControlCode, 6> mappings{
				{ Keys::C,			TextControlCode::Copy },
				{ Keys::V,			TextControlCode::Paste },
				{ Keys::X,			TextControlCode::Cut },
				{ Keys::Z,			TextControlCode::Undo },
				{ Keys::Y,			TextControlCode::Redo },
				{ Keys::A,			TextControlCode::SelectAll },
				{ Keys::Insert,		TextControlCode::Paste },
			};

			if (mappings.count( scancode ))
				control_code = mappings.at( scancode );
		}

		if (is_shift_down && !is_ctrl_down)
		{
			static constexpr frozen::unordered_map<ButtonCode_T, TextControlCode, 4> mappings{
				{ Keys::Left,	TextControlCode::SelectLeft },
				{ Keys::Right,	TextControlCode::SelectRight },
				{ Keys::Up,		TextControlCode::SelectUp },
				{ Keys::Down,	TextControlCode::SelectDown },
			};

			if (mappings.count( scancode ))
				control_code = mappings.at( scancode );
		}

		if (is_shift_down && is_ctrl_down)
		{
			static constexpr frozen::unordered_map<ButtonCode_T, TextControlCode, 1> mappings{
				{ Keys::Z,	TextControlCode::Redo },
			};

			if (mappings.count( scancode ))
				control_code = mappings.at( scancode );
		}

		if (control_code.has_value())
			OnTextControlCodeGenerated( control_code.value() );

		InputButtonDevice::OnButtonPressed( scancode );
	}

	void KeyboardInput::OnButtonReleased( ButtonCode_T scancode )
	{
		InputButtonDevice::OnButtonReleased( scancode );
	}

	void KeyboardInput::OnTextControlCodeGenerated( TextControlCode )
	{
	}
}
