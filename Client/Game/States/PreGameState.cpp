#include "PreGameState.hpp"

namespace Game::States
{
	fsm::Might<fsm::TransitionTo<MainMenuState>> PreGameState::HandleEvent( const Events::FrameEvent& )
	{
		// TODO: only transition after we've loaded starting stuff
		return fsm::TransitionTo<MainMenuState>();
	}
}
