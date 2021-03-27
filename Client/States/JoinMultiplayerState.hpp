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
		: public StateMachine::DefaultAction<StateMachine::Actions::NoAction>
		, NonCopyable
	{
		using ExitActions = StateMachine::Actions::OneOf< StateMachine::Actions::NoAction,
			StateMachine::Actions::TransitionTo<MainMenuState>,
			StateMachine::Actions::TransitionTo<LoadingState>
		>;

	public:
		using StateMachine::DefaultAction<StateMachine::Actions::NoAction>::HandleEvent;

		explicit JoinMultiplayerState();
		virtual ~JoinMultiplayerState();

		ExitActions HandleEvent( const FrameEvent& e );
		ExitActions HandleEvent( const DearImGuiFrameEvent& e );

	protected:
		void InitiateConnection( std::string address );
		void CancelConnection();

	protected:
		std::unique_ptr<Sessions::ClientServerSession> server_connection;
		std::string status_message;
	};
}
