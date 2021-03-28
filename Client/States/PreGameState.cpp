#include "PreGameState.hpp"

namespace ClientStates
{
	PreGameState::PreGameState()
	{
	}

	PreGameState::~PreGameState()
	{
	}

	fsm::Actions::Might<fsm::Actions::TransitionTo<MainMenuState>> PreGameState::HandleEvent( const FrameEvent& e )
	{
		(void)e;

		// TODO: only transition after we've loaded starting stuff
		return fsm::Actions::TransitionTo<MainMenuState>();
	}
}
