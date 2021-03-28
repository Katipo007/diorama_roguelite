#include "LoadingState.hpp"

#include "Client/ClientGame.hpp"

namespace ClientStates
{
	LoadingState::LoadingState()
	{
	}

	LoadingState::~LoadingState()
	{
	}

	fsm::Actions::NoAction LoadingState::OnEnter( const ConnectedToServerEvent& e )
	{
		(void)e;
		return fsm::Actions::NoAction{};
	}

	fsm::Actions::Might<
		  fsm::Actions::TransitionTo<MainMenuState>
		, fsm::Actions::TransitionTo<InGameState>
	> LoadingState::HandleEvent( const FrameEvent& e )
	{
		(void)e;

		// TODO: load stuff

		return fsm::Actions::TransitionTo<InGameState>{};
	}

	fsm::Actions::TransitionTo<MainMenuState> LoadingState::HandleEvent( const DisconnectedFromServerEvent& e )
	{
		(void)e;
		LOG_INFO( Client, "Lost connection to server while loading, returning to main menu" );
		return fsm::Actions::TransitionTo<MainMenuState>();
	}
}
