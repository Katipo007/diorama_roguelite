#pragma once

#include "Client/States/Events.hpp"
#include "Common/Utility/StateMachine/DefaultAction.hpp"
#include "Common/Utility/StateMachine/Actions/Might.hpp"
#include "Common/Utility/StateMachine/Actions/TransitionTo.hpp"

namespace Game
{
	class ClientGame;
}

namespace ClientStates
{
	class MainMenuState;
	class LoadingState;

	/// <summary>
	/// Screen to join an existing multi-player game
	/// </summary>
	class JoinMultiplayerState
		: public fsm::DefaultAction<fsm::Actions::NoAction>
	{
		using ExitActions = fsm::Actions::OneOf< fsm::Actions::NoAction,
			fsm::Actions::TransitionTo<MainMenuState>,
			fsm::Actions::TransitionTo<LoadingState>
		>;

	public:
		using fsm::DefaultAction<fsm::Actions::NoAction>::HandleEvent;

		explicit JoinMultiplayerState( Game::ClientGame& client );
		explicit JoinMultiplayerState( JoinMultiplayerState&& to_move ) = default;
		virtual ~JoinMultiplayerState();

		ExitActions HandleEvent( const FrameEvent& e );
		ExitActions HandleEvent( const DearImGuiFrameEvent& e );
		fsm::Actions::TransitionTo<LoadingState> HandleEvent( const ConnectedToServerEvent& e );

	protected:
		void InitiateConnection( std::string address );
		void CancelConnection();

	protected:
		Game::ClientGame& client;

		std::string status_message;

	private:
		JoinMultiplayerState( const JoinMultiplayerState& ) = delete;
		JoinMultiplayerState operator=( const JoinMultiplayerState& ) = delete;
	};
}
