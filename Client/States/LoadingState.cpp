#include "LoadingState.hpp"

namespace ClientStates
{
	LoadingState::LoadingState()
	{
	}

	LoadingState::~LoadingState()
	{
	}

	StateMachine::Actions::Might<StateMachine::Actions::TransitionTo<MainMenuState>, StateMachine::Actions::TransitionTo<InGameState>> LoadingState::HandleEvent( const FrameEvent& e )
	{
		(void)e;

		// TODO: return to main menu on fail

		return StateMachine::Actions::TransitionTo<InGameState>{};
	}
}
