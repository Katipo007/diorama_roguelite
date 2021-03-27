#pragma once

#include "Client/States/Events.hpp"
#include "Common/Utility/StateMachine/DefaultAction.hpp"
#include "Common/Utility/StateMachine/Actions/Might.hpp"
#include "Common/Utility/StateMachine/Actions/TransitionTo.hpp"
#include "Common/Utility/NonCopyable.hpp"

namespace ClientStates
{
	class JoinMultiplayerState;

	/// <summary>
	/// Main menu
	/// </summary>
	class MainMenuState
		: public StateMachine::DefaultAction<StateMachine::Actions::NoAction>
		, NonCopyable
	{
		using OutTransitionActions = StateMachine::Actions::Might<
			StateMachine::Actions::TransitionTo<JoinMultiplayerState>
		>;

	public:
		using StateMachine::DefaultAction<StateMachine::Actions::NoAction>::HandleEvent;

		explicit MainMenuState();
		virtual ~MainMenuState();

		StateMachine::Actions::NoAction HandleEvent( const FrameEvent& e );
		OutTransitionActions HandleEvent( const DearImGuiFrameEvent& e );

	protected:
	};
}
