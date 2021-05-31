#include "KeyboardInputSDL2.hpp"

#include "Common/Core/Input/Keycodes.hpp"
#include "Common/Utility/Unreachable.hpp"
#include "SDL2Include.hpp"

namespace Plugins
{
	KeyboardInputSDL2::KeyboardInputSDL2()
		: KeyboardInput( SDL_NUM_SCANCODES )
	{
		SDL_StartTextInput();
	}

	KeyboardInputSDL2::~KeyboardInputSDL2()
	{
	}

	std::string_view KeyboardInputSDL2::GetButtonName( Input::ButtonCode_T scancode ) const
	{
		switch (scancode)
		{
		case Input::Keys::Backspace:	return "Backspace";
		case Input::Keys::Escape:		return "Escape";
		case Input::Keys::Delete:		return "Delete";

		default:
			if ((scancode >= Input::Keys::A) && (scancode <= Input::Keys::Z))
			{
				thread_local static char letter;
				letter = static_cast<char>(scancode - Input::Keys::A) + 'A';
				return { &letter, &letter + 1 };
			}
			else
			{
				return SDL_GetKeyName( SDL_Keycode( scancode ) );
			}
			break;
		}
	}

	void KeyboardInputSDL2::Update()
	{
		ClearButtonPresses(); // reset each frame
	}

	void KeyboardInputSDL2::ProcessEvent( const SDL_KeyboardEvent& e )
	{
		if (e.type == SDL_KEYUP)
			OnButtonReleased( e.keysym.scancode );
		else if (e.type == SDL_KEYDOWN)
			OnButtonPressed( e.keysym.scancode );
	}

	void KeyboardInputSDL2::ProcessEvent( const SDL_TextEditingEvent& )
	{
		// TODO
	}

	void KeyboardInputSDL2::ProcessEvent( const SDL_TextInputEvent& e )
	{
		OnTextEntered( e.text );
	}

	void KeyboardInputSDL2::OnTextEntered( std::string_view )
	{
		// TODO
	}

	void KeyboardInputSDL2::OnTextControlCodeGenerated( Input::TextControlCode )
	{
		
	}
}
