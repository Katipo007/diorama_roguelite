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
		: public fsm::DefaultAction<fsm::Actions::NoAction>
		, NonCopyable
	{
		using OutTransitionActions = fsm::Actions::Might<
			fsm::Actions::TransitionTo<JoinMultiplayerState>
		>;

	public:
		using fsm::DefaultAction<fsm::Actions::NoAction>::HandleEvent;

		explicit MainMenuState();
		virtual ~MainMenuState();

		fsm::Actions::NoAction HandleEvent( const FrameEvent& e );
		OutTransitionActions HandleEvent( const DearImGuiFrameEvent& e );

	protected:
	};
}
