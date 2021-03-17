
#ifdef PLATFORM_WINDOWS

#include "Visual/Input/Input.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#ifdef _WIN64
#	pragma comment(lib, "Visual/Vendor/SDL2/lib/x64/SDL2.lib")
#else
#	pragma comment(lib, "Visual/Vendor/SDL2/lib/x86/SDL2.lib")
#endif

namespace
{
	SDL_KeyCode KeyCodeToSdlKeycode( const ::Input::KeyCode key )
	{
		switch (key)
		{
		case ::Input::Key::Return: return SDLK_RETURN;

		case ::Input::Key::Escape: return SDLK_ESCAPE;
		case ::Input::Key::Backspace: return SDLK_BACKSPACE;
		case ::Input::Key::Tab: return SDLK_TAB;
		case ::Input::Key::Space: return SDLK_SPACE;
		case ::Input::Key::Exclimation: return SDLK_EXCLAIM;
		case ::Input::Key::DoubleQuote: return SDLK_QUOTEDBL;
		case ::Input::Key::Hash: return SDLK_HASH;
		case ::Input::Key::Percent: return SDLK_PERCENT;
		case ::Input::Key::Dollar: return SDLK_DOLLAR;
		case ::Input::Key::Ampersand: return SDLK_AMPERSAND;
		case ::Input::Key::Quote: return SDLK_QUOTE;
		case ::Input::Key::LeftParenthesis: return SDLK_LEFTPAREN;
		case ::Input::Key::RightParenthesis: return SDLK_RIGHTPAREN;
		case ::Input::Key::Asterisk: return SDLK_ASTERISK;
		case ::Input::Key::Plus: return SDLK_PLUS;
		case ::Input::Key::Comma: return SDLK_COMMA;
		case ::Input::Key::Minus: return SDLK_MINUS;
		case ::Input::Key::Period: return SDLK_PERIOD;
		case ::Input::Key::ForwardSlash: return SDLK_SLASH;
		case ::Input::Key::Digit0: return SDLK_0;
		case ::Input::Key::Digit1: return SDLK_1;
		case ::Input::Key::Digit2: return SDLK_2;
		case ::Input::Key::Digit3: return SDLK_3;
		case ::Input::Key::Digit4: return SDLK_4;
		case ::Input::Key::Digit5: return SDLK_5;
		case ::Input::Key::Digit6: return SDLK_6;
		case ::Input::Key::Digit7: return SDLK_7;
		case ::Input::Key::Digit8: return SDLK_8;
		case ::Input::Key::Digit9: return SDLK_9;
		case ::Input::Key::Colon: return SDLK_COLON;
		case ::Input::Key::SemiColon: return SDLK_SEMICOLON;
		case ::Input::Key::Less: return SDLK_LESS;
		case ::Input::Key::Equals: return SDLK_EQUALS;
		case ::Input::Key::Greater: return SDLK_GREATER;
		case ::Input::Key::QuestionMark: return SDLK_QUESTION;
		case ::Input::Key::At: return SDLK_AT;

		case ::Input::Key::LeftBracket: return SDLK_LEFTBRACKET;
		case ::Input::Key::BackSlash: return SDLK_BACKSLASH;
		case ::Input::Key::RightBracket: return SDLK_RIGHTBRACKET;
		case ::Input::Key::Caret: return SDLK_CARET;
		case ::Input::Key::Underscore: return SDLK_UNDERSCORE;
		case ::Input::Key::Backquote:return SDLK_BACKQUOTE;
		case ::Input::Key::a: return SDLK_a;
		case ::Input::Key::b: return SDLK_b;
		case ::Input::Key::c: return SDLK_c;
		case ::Input::Key::d: return SDLK_d;
		case ::Input::Key::e: return SDLK_e;
		case ::Input::Key::f: return SDLK_f;
		case ::Input::Key::g: return SDLK_g;
		case ::Input::Key::h: return SDLK_h;
		case ::Input::Key::i: return SDLK_i;
		case ::Input::Key::j: return SDLK_j;
		case ::Input::Key::k: return SDLK_k;
		case ::Input::Key::l: return SDLK_l;
		case ::Input::Key::m: return SDLK_m;
		case ::Input::Key::n: return SDLK_n;
		case ::Input::Key::o: return SDLK_o;
		case ::Input::Key::p: return SDLK_p;
		case ::Input::Key::q: return SDLK_q;
		case ::Input::Key::r: return SDLK_r;
		case ::Input::Key::s: return SDLK_s;
		case ::Input::Key::t: return SDLK_t;
		case ::Input::Key::u: return SDLK_u;
		case ::Input::Key::v: return SDLK_v;
		case ::Input::Key::w: return SDLK_w;
		case ::Input::Key::x: return SDLK_x;
		case ::Input::Key::y: return SDLK_y;
		case ::Input::Key::z: return SDLK_z;

		case ::Input::Key::CapsLock: return SDLK_CAPSLOCK;

		case ::Input::Key::F1:  return SDLK_F1;
		case ::Input::Key::F2:  return SDLK_F2;
		case ::Input::Key::F3:  return SDLK_F3;
		case ::Input::Key::F4:  return SDLK_F4;
		case ::Input::Key::F5:  return SDLK_F5;
		case ::Input::Key::F6:  return SDLK_F6;
		case ::Input::Key::F7:  return SDLK_F7;
		case ::Input::Key::F8:  return SDLK_F8;
		case ::Input::Key::F9:  return SDLK_F9;
		case ::Input::Key::F10: return SDLK_F10;
		case ::Input::Key::F11: return SDLK_F11;
		case ::Input::Key::F12: return SDLK_F12;

		case ::Input::Key::PrintScreen: return SDLK_PRINTSCREEN;
		case ::Input::Key::ScrollLock: return SDLK_SCROLLLOCK;
		case ::Input::Key::Pause: return SDLK_PAUSE;
		case ::Input::Key::Insert: return SDLK_INSERT;
		case ::Input::Key::Home: return SDLK_HOME;
		case ::Input::Key::PageUp: return SDLK_PAGEUP;
		case ::Input::Key::Delete: return SDLK_DELETE;
		case ::Input::Key::End: return SDLK_END;
		case ::Input::Key::PageDown: return SDLK_PAGEDOWN;
		case ::Input::Key::Right: return SDLK_RIGHT;
		case ::Input::Key::Left: return SDLK_LEFT;
		case ::Input::Key::Down: return SDLK_DOWN;
		case ::Input::Key::Up: return SDLK_UP;

		case ::Input::Key::NumLockClear: return SDLK_NUMLOCKCLEAR;
		case ::Input::Key::NumPad_Divide: return SDLK_KP_DIVIDE;
		case ::Input::Key::NumPad_Multiply: return SDLK_KP_MULTIPLY;
		case ::Input::Key::NumPad_Minus: return SDLK_KP_MINUS;
		case ::Input::Key::NumPad_Plus: return SDLK_KP_PLUS;
		case ::Input::Key::NumPad_Enter: return SDLK_KP_ENTER;
		case ::Input::Key::NumPad_1: return SDLK_KP_1;
		case ::Input::Key::NumPad_2: return SDLK_KP_2;
		case ::Input::Key::NumPad_3:	return SDLK_KP_3;
		case ::Input::Key::NumPad_4:	return SDLK_KP_4;
		case ::Input::Key::NumPad_5:	return SDLK_KP_5;
		case ::Input::Key::NumPad_6:	return SDLK_KP_6;
		case ::Input::Key::NumPad_7:	return SDLK_KP_7;
		case ::Input::Key::NumPad_8:	return SDLK_KP_8;
		case ::Input::Key::NumPad_9:	return SDLK_KP_9;
		case ::Input::Key::NumPad_0:	return SDLK_KP_0;
		case ::Input::Key::NumPad_Period: return SDLK_KP_PERIOD;

		case ::Input::Key::AltErase: return SDLK_ALTERASE;

		case ::Input::Key::LeftCtrl: return SDLK_LCTRL;
		case ::Input::Key::LeftShift: return SDLK_LSHIFT;
		case ::Input::Key::LeftAlt: return SDLK_LALT;
		case ::Input::Key::LeftSuper: return SDLK_LGUI;
		case ::Input::Key::RightCtrl: return SDLK_RCTRL;
		case ::Input::Key::RightShift: return SDLK_RSHIFT;
		case ::Input::Key::RightAlt: return SDLK_RALT;
		case ::Input::Key::RightSuper: return SDLK_RGUI;

		default:
			return SDLK_UNKNOWN;
		}
	}

	SDL_Scancode KeyCodeToSdlScancode( const ::Input::KeyCode key )
	{
		switch (key)
		{
		case ::Input::Key::Return: return SDL_SCANCODE_RETURN;

		case ::Input::Key::Escape: return SDL_SCANCODE_ESCAPE;
		case ::Input::Key::Backspace: return SDL_SCANCODE_BACKSPACE;
		case ::Input::Key::Tab: return SDL_SCANCODE_TAB;
		case ::Input::Key::Space: return SDL_SCANCODE_SPACE;
		case ::Input::Key::Exclimation: return SDL_SCANCODE_1;
		case ::Input::Key::Comma: return SDL_SCANCODE_COMMA;
		case ::Input::Key::Minus: return SDL_SCANCODE_MINUS;
		case ::Input::Key::Period: return SDL_SCANCODE_PERIOD;
		case ::Input::Key::ForwardSlash: return SDL_SCANCODE_SLASH;
		case ::Input::Key::Digit0: return SDL_SCANCODE_0;
		case ::Input::Key::Digit1: return SDL_SCANCODE_1;
		case ::Input::Key::Digit2: return SDL_SCANCODE_2;
		case ::Input::Key::Digit3: return SDL_SCANCODE_3;
		case ::Input::Key::Digit4: return SDL_SCANCODE_4;
		case ::Input::Key::Digit5: return SDL_SCANCODE_5;
		case ::Input::Key::Digit6: return SDL_SCANCODE_6;
		case ::Input::Key::Digit7: return SDL_SCANCODE_7;
		case ::Input::Key::Digit8: return SDL_SCANCODE_8;
		case ::Input::Key::Digit9: return SDL_SCANCODE_9;
		case ::Input::Key::SemiColon: return SDL_SCANCODE_SEMICOLON;
		case ::Input::Key::Equals: return SDL_SCANCODE_EQUALS;

		case ::Input::Key::LeftBracket: return SDL_SCANCODE_LEFTBRACKET;
		case ::Input::Key::BackSlash: return SDL_SCANCODE_BACKSLASH;
		case ::Input::Key::RightBracket: return SDL_SCANCODE_RIGHTBRACKET;
		case ::Input::Key::Caret: return SDL_SCANCODE_GRAVE;
		case ::Input::Key::a: return SDL_SCANCODE_A;
		case ::Input::Key::b: return SDL_SCANCODE_B;
		case ::Input::Key::c: return SDL_SCANCODE_C;
		case ::Input::Key::d: return SDL_SCANCODE_D;
		case ::Input::Key::e: return SDL_SCANCODE_E;
		case ::Input::Key::f: return SDL_SCANCODE_F;
		case ::Input::Key::g: return SDL_SCANCODE_G;
		case ::Input::Key::h: return SDL_SCANCODE_H;
		case ::Input::Key::i: return SDL_SCANCODE_I;
		case ::Input::Key::j: return SDL_SCANCODE_J;
		case ::Input::Key::k: return SDL_SCANCODE_K;
		case ::Input::Key::l: return SDL_SCANCODE_L;
		case ::Input::Key::m: return SDL_SCANCODE_M;
		case ::Input::Key::n: return SDL_SCANCODE_N;
		case ::Input::Key::o: return SDL_SCANCODE_O;
		case ::Input::Key::p: return SDL_SCANCODE_P;
		case ::Input::Key::q: return SDL_SCANCODE_Q;
		case ::Input::Key::r: return SDL_SCANCODE_R;
		case ::Input::Key::s: return SDL_SCANCODE_S;
		case ::Input::Key::t: return SDL_SCANCODE_T;
		case ::Input::Key::u: return SDL_SCANCODE_U;
		case ::Input::Key::v: return SDL_SCANCODE_V;
		case ::Input::Key::w: return SDL_SCANCODE_W;
		case ::Input::Key::x: return SDL_SCANCODE_X;
		case ::Input::Key::y: return SDL_SCANCODE_Y;
		case ::Input::Key::z: return SDL_SCANCODE_Z;

		case ::Input::Key::CapsLock: return SDL_SCANCODE_CAPSLOCK;

		case ::Input::Key::F1:  return SDL_SCANCODE_F1;
		case ::Input::Key::F2:  return SDL_SCANCODE_F2;
		case ::Input::Key::F3:  return SDL_SCANCODE_F3;
		case ::Input::Key::F4:  return SDL_SCANCODE_F4;
		case ::Input::Key::F5:  return SDL_SCANCODE_F5;
		case ::Input::Key::F6:  return SDL_SCANCODE_F6;
		case ::Input::Key::F7:  return SDL_SCANCODE_F7;
		case ::Input::Key::F8:  return SDL_SCANCODE_F8;
		case ::Input::Key::F9:  return SDL_SCANCODE_F9;
		case ::Input::Key::F10: return SDL_SCANCODE_F10;
		case ::Input::Key::F11: return SDL_SCANCODE_F11;
		case ::Input::Key::F12: return SDL_SCANCODE_F12;

		case ::Input::Key::PrintScreen: return SDL_SCANCODE_PRINTSCREEN;
		case ::Input::Key::ScrollLock: return SDL_SCANCODE_SCROLLLOCK;
		case ::Input::Key::Pause: return SDL_SCANCODE_PAUSE;
		case ::Input::Key::Insert: return SDL_SCANCODE_INSERT;
		case ::Input::Key::Home: return SDL_SCANCODE_HOME;
		case ::Input::Key::PageUp: return SDL_SCANCODE_PAGEUP;
		case ::Input::Key::Delete: return SDL_SCANCODE_DELETE;
		case ::Input::Key::End: return SDL_SCANCODE_END;
		case ::Input::Key::PageDown: return SDL_SCANCODE_PAGEDOWN;
		case ::Input::Key::Right: return SDL_SCANCODE_RIGHT;
		case ::Input::Key::Left: return SDL_SCANCODE_LEFT;
		case ::Input::Key::Down: return SDL_SCANCODE_DOWN;
		case ::Input::Key::Up: return SDL_SCANCODE_UP;

		case ::Input::Key::NumLockClear: return SDL_SCANCODE_NUMLOCKCLEAR;
		case ::Input::Key::NumPad_Divide: return SDL_SCANCODE_KP_DIVIDE;
		case ::Input::Key::NumPad_Multiply: return SDL_SCANCODE_KP_MULTIPLY;
		case ::Input::Key::NumPad_Minus: return SDL_SCANCODE_KP_MINUS;
		case ::Input::Key::NumPad_Plus: return SDL_SCANCODE_KP_PLUS;
		case ::Input::Key::NumPad_Enter: return SDL_SCANCODE_KP_ENTER;
		case ::Input::Key::NumPad_1: return SDL_SCANCODE_KP_1;
		case ::Input::Key::NumPad_2: return SDL_SCANCODE_KP_2;
		case ::Input::Key::NumPad_3:	return SDL_SCANCODE_KP_3;
		case ::Input::Key::NumPad_4:	return SDL_SCANCODE_KP_4;
		case ::Input::Key::NumPad_5:	return SDL_SCANCODE_KP_5;
		case ::Input::Key::NumPad_6:	return SDL_SCANCODE_KP_6;
		case ::Input::Key::NumPad_7:	return SDL_SCANCODE_KP_7;
		case ::Input::Key::NumPad_8:	return SDL_SCANCODE_KP_8;
		case ::Input::Key::NumPad_9:	return SDL_SCANCODE_KP_9;
		case ::Input::Key::NumPad_0:	return SDL_SCANCODE_KP_0;
		case ::Input::Key::NumPad_Period: return SDL_SCANCODE_KP_PERIOD;

		case ::Input::Key::AltErase: return SDL_SCANCODE_ALTERASE;

		case ::Input::Key::LeftCtrl: return SDL_SCANCODE_LCTRL;
		case ::Input::Key::LeftShift: return SDL_SCANCODE_LSHIFT;
		case ::Input::Key::LeftAlt: return SDL_SCANCODE_LALT;
		case ::Input::Key::LeftSuper: return SDL_SCANCODE_LGUI;
		case ::Input::Key::RightCtrl: return SDL_SCANCODE_RCTRL;
		case ::Input::Key::RightShift: return SDL_SCANCODE_RSHIFT;
		case ::Input::Key::RightAlt: return SDL_SCANCODE_RALT;
		case ::Input::Key::RightSuper: return SDL_SCANCODE_RGUI;

		default:
			return SDL_SCANCODE_UNKNOWN;
		}
	}

	SDL_Window* window_handle = NULL;
}

bool InputManager::IsKeyPressed( const ::Input::KeyCode key )
{
	ASSERT( window_handle != NULL );

	int num_keys;
	const Uint8* keyboard_state = SDL_GetKeyboardState( &num_keys );

	const auto sdl_scancode = KeyCodeToSdlScancode( key );
	ASSERT( sdl_scancode != SDLK_UNKNOWN );
	ASSERT( sdl_scancode < num_keys );

	return keyboard_state[sdl_scancode] & SDL_PRESSED;
}

bool InputManager::IsMouseButtonPressed( const ::Input::MouseCode button )
{
	ASSERT( window_handle != NULL );

	auto mouse_state = SDL_GetMouseState( NULL, NULL );

	switch (button)
	{
	case ::Input::Mouse::ButtonLeft: return mouse_state & SDL_BUTTON_LMASK;
	case ::Input::Mouse::ButtonRight: return mouse_state & SDL_BUTTON_RMASK;
	case ::Input::Mouse::ButtonMiddle: return mouse_state & SDL_BUTTON_MMASK;

	default:
		ASSERT( false, "Unexpected MouseCode" );
		return false;
	}
}

std::pair<float, float> InputManager::GetMousePosition()
{
	ASSERT( window_handle != NULL );

	int x, y;
	auto mouse_state = SDL_GetMouseState( &x, &y );
	(void)mouse_state;

	return { (float)x, (float)y };
}

float InputManager::GetMouseX()
{
	return GetMousePosition().first;
}

float InputManager::GetMouseY()
{
	return GetMousePosition().second;
}

void InputManager::SetWindowHandle( void* new_window_handle )
{
	window_handle = static_cast<SDL_Window*>(new_window_handle);
}

#endif

