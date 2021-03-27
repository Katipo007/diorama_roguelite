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
		: public StateMachine::DefaultAction<StateMachine::Actions::NoAction>
		, NonCopyable
	{
	public:
		using StateMachine::DefaultAction<StateMachine::Actions::NoAction>::HandleEvent;

		explicit LoadingState();
		virtual ~LoadingState();

		StateMachine::Actions::Might<
			StateMachine::Actions::TransitionTo<MainMenuState>, // could return to menu if something fails to load or the connection was severed
			StateMachine::Actions::TransitionTo<InGameState> // transition into in-game
		> HandleEvent( const FrameEvent& e );

	protected:
	};
}
