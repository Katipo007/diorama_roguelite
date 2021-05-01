#pragma once

#include "Client/States/Events.hpp"
#include "Common/Utility/StateMachine/DefaultAction.hpp"
#include "Common/Utility/StateMachine/Actions/Might.hpp"
#include "Common/Utility/StateMachine/Actions/TransitionTo.hpp"
#include "Common/Utility/NonCopyable.hpp"

namespace ClientStates
{
	class MainMenuState;
	class InGameState;

	/// <summary>
	/// Loading screen going from menu to in-game
	/// </summary>
	class LoadingState
		: public fsm::DefaultAction<fsm::Actions::NoAction>
	{
	public:
		using fsm::DefaultAction<fsm::Actions::NoAction>::HandleEvent;

		explicit LoadingState();
		explicit LoadingState( LoadingState&& to_move ) = default;
		virtual ~LoadingState();

		fsm::Actions::NoAction OnEnter( const ConnectedToServerEvent& e );

		fsm::Actions::Might<
			  fsm::Actions::TransitionTo<MainMenuState>
			, fsm::Actions::TransitionTo<InGameState>
		> HandleEvent( const FrameEvent& e );

		fsm::Actions::TransitionTo<MainMenuState> HandleEvent( const DisconnectedFromServerEvent& e );

	protected:

	private:
		LoadingState( const LoadingState& ) = delete;
		LoadingState operator=( const LoadingState& ) = delete;
	};
}
