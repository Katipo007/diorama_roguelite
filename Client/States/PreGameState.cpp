#include "Precompiled\pch.hpp"
#include "PreGameState.hpp"

namespace ClientStates
{
	PreGameState::PreGameState()
	{
	}

	PreGameState::~PreGameState()
	{
	}

	StateMachine::Actions::Might<StateMachine::Actions::TransitionTo<InGameState>> PreGameState::HandleEvent( const FrameEvent& e )
	{
		(void)e;

		// TODO: only transition after we've loaded starting stuff
		return StateMachine::Actions::TransitionTo<InGameState>();
	}
}
