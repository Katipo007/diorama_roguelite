#pragma once

#include "Common/Core/Input/KeyboardInput.hpp"

struct SDL_KeyboardEvent;
struct SDL_TextEditingEvent;
struct SDL_TextInputEvent;

namespace Plugins
{
	class InputSDL2;

	class KeyboardInputSDL2 final
		: public Input::KeyboardInput
	{
		friend class InputSDL2;

	public:
		~KeyboardInputSDL2();

		std::string_view GetButtonName( Input::ButtonCode_T scancode ) const override;

		void Update();

	private:
		KeyboardInputSDL2();

		void ProcessEvent( const SDL_KeyboardEvent& e );
		void ProcessEvent( const SDL_TextEditingEvent& e );
		void ProcessEvent( const SDL_TextInputEvent& e );

		void OnTextEntered( std::string_view text );
		void OnTextControlCodeGenerated( Input::TextControlCode control_code ) override;
	};
}
