#pragma once

#include "Client/States/Events.hpp"
#include "Common/Utility/StateMachine/DefaultAction.hpp"
#include "Common/Utility/StateMachine/Actions/Might.hpp"
#include "Common/Utility/StateMachine/Actions/TransitionTo.hpp"
#include "Common/Utility/NonCopyable.hpp"

namespace ClientStates
{
	class MainMenuState;
	class LoadingState;

	/// <summary>
	/// Screen to join an existing multi-player game
	/// </summary>
	class JoinMultiplayerState
		: public fsm::DefaultAction<fsm::Actions::NoAction>
		, NonCopyable
	{
		using ExitActions = fsm::Actions::OneOf< fsm::Actions::NoAction,
			fsm::Actions::TransitionTo<MainMenuState>,
			fsm::Actions::TransitionTo<LoadingState>
		>;

	public:
		using fsm::DefaultAction<fsm::Actions::NoAction>::HandleEvent;

		explicit JoinMultiplayerState();
		virtual ~JoinMultiplayerState();

		ExitActions HandleEvent( const FrameEvent& e );
		ExitActions HandleEvent( const DearImGuiFrameEvent& e );
		fsm::Actions::TransitionTo<LoadingState> HandleEvent( const ConnectedToServerEvent& e );

	protected:
		void InitiateConnection( std::string address );
		void CancelConnection();

	protected:
		std::string status_message;
	};
}
