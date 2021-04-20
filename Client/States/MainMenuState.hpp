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
	class JoinMultiplayerState;

	/// <summary>
	/// Main menu
	/// </summary>
	class MainMenuState
		: public fsm::DefaultAction<fsm::Actions::NoAction>
	{
		using OutTransitionActions = fsm::Actions::Might<
			fsm::Actions::TransitionTo<JoinMultiplayerState>
		>;

	public:
		using fsm::DefaultAction<fsm::Actions::NoAction>::HandleEvent;

		explicit MainMenuState( Game::ClientGame& client );
		explicit MainMenuState( MainMenuState&& to_move ) = default;
		virtual ~MainMenuState();

		fsm::Actions::NoAction HandleEvent( const FrameEvent& e );
		OutTransitionActions HandleEvent( const DearImGuiFrameEvent& e );

	protected:
		Game::ClientGame& client;

	private:
		MainMenuState( const MainMenuState& ) = delete;
		MainMenuState operator=( const MainMenuState& ) = delete;
	};
}
