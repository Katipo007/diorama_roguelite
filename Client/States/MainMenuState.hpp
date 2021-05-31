#pragma once

#include "Client/States/Events.hpp"
#include "Common/Utility/StateMachine/DefaultAction.hpp"
#include "Common/Utility/StateMachine/Might.hpp"
#include "Common/Utility/StateMachine/TransitionTo.hpp"

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
		: public fsm::DefaultAction<fsm::NoAction>
	{
		using OutTransitionActions = fsm::Might<
			fsm::TransitionTo<JoinMultiplayerState>
		>;

	public:
		using fsm::DefaultAction<fsm::NoAction>::HandleEvent;

		explicit MainMenuState( Game::ClientGame& client );
		explicit MainMenuState( MainMenuState&& to_move ) = default;
		virtual ~MainMenuState();

		fsm::NoAction HandleEvent( const FrameEvent& e );
		OutTransitionActions HandleEvent( const DearImGuiFrameEvent& e );

	protected:
		Game::ClientGame& client;

	private:
		MainMenuState( const MainMenuState& ) = delete;
		MainMenuState operator=( const MainMenuState& ) = delete;
	};
}
