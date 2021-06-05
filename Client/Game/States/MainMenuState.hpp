#pragma once

#include "Events.hpp"
#include "Common/Utility/StateMachine/DefaultAction.hpp"
#include "Common/Utility/StateMachine/Might.hpp"
#include "Common/Utility/StateMachine/TransitionTo.hpp"
#include "Common/Utility/NonCopyable.hpp"

namespace Game::States
{
	class ExitGameState;
	class JoinMultiplayerState;

	/// <summary>
	/// Main menu
	/// </summary>
	class MainMenuState
		: public fsm::DefaultAction<fsm::NoAction>
		, NonCopyable
	{

	public:
		using fsm::DefaultAction<fsm::NoAction>::HandleEvent;

		fsm::Might<fsm::TransitionTo<JoinMultiplayerState>, fsm::TransitionTo<ExitGameState>> HandleEvent( const Events::DearImGuiFrameEvent& e );
	};
}
