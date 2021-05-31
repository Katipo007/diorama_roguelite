#pragma once

#include "Common/Core/API/InputAPI.hpp"

namespace API { class SystemAPI; }

struct SDL_KeyboardEvent;
struct SDL_TextEditingEvent;
struct SDL_TextInputEvent;
struct SDL_MouseMotionEvent;
struct SDL_MouseButtonEvent;
struct SDL_MouseWheelEvent;

namespace Plugins
{
	class KeyboardInputSDL2;

	class InputSDL2 final
		: public API::InputAPI
	{
	public:
		explicit InputSDL2( API::SystemAPI& system );
		~InputSDL2();

		std::string_view GetName() const noexcept override { return "SDL2 Input"; }

		size_t GetKeyboardCount() const override;
		std::shared_ptr<Input::KeyboardInput> GetKeyboard( size_t idx ) const override;

		void ProcessEvent( SDL_KeyboardEvent& e );
		void ProcessEvent( SDL_TextEditingEvent& e );
		void ProcessEvent( SDL_TextInputEvent& e );

	private:
		void Init() override;
		void Shutdown() override;
		void BeginEvents( const PreciseTimestep& ts ) override;

	private:
		API::SystemAPI& system;

		std::vector<std::shared_ptr<KeyboardInputSDL2>> keyboards;
	};
}
