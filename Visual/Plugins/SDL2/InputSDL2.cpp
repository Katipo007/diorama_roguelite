#include "InputSDL2.hpp"

#include "Common/Core/API/SystemAPI.hpp"
#include "KeyboardInputSDL2.hpp"
#include "SDL2Include.hpp"

namespace Plugins
{
	InputSDL2::InputSDL2( API::SystemAPI& system )
		: system( system )
	{

	}

	InputSDL2::~InputSDL2() = default;

	void InputSDL2::Init()
	{
		keyboards.emplace_back( new KeyboardInputSDL2() );
	}

	void InputSDL2::Shutdown()
	{
		keyboards.clear();
	}
	void InputSDL2::BeginEvents( const PreciseTimestep& )
	{
		for (auto& keyboard : keyboards)
			keyboard->Update();
	}


	size_t InputSDL2::GetKeyboardCount() const
	{
		return keyboards.size();
	}

	std::shared_ptr<Input::KeyboardInput> InputSDL2::GetKeyboard( size_t idx ) const
	{
		return keyboards.at( idx );
	}

	void InputSDL2::ProcessEvent( SDL_KeyboardEvent& e )
	{
		switch (e.type)
		{
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			for (auto& keyboard : keyboards)
				keyboard->ProcessEvent( e );
			break;
		}
	}

	void InputSDL2::ProcessEvent( SDL_TextEditingEvent& e )
	{
		switch (e.type)
		{
		case SDL_TEXTEDITING:
			for (auto& keyboard : keyboards)
				keyboard->ProcessEvent( e );
			break;
		}
	}

	void InputSDL2::ProcessEvent( SDL_TextInputEvent& e )
	{
		switch (e.type)
		{
		case SDL_TEXTINPUT:
			for (auto& keyboard : keyboards)
				keyboard->ProcessEvent( e );
			break;
		}
	}
}
