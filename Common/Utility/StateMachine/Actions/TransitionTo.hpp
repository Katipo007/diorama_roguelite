#pragma once

#include "NoAction.hpp"

namespace StateMachine::Actions
{
	/// <summary>
	/// Have the state machine transition to the given state
	/// </summary>
	/// <typeparam name="TargetState">State type to transition to</typeparam>
	template<typename TargetState>
	class TransitionTo
	{
	public:
		template<typename Machine, typename PreviousState, typename Event>
		void Execute( Machine& machine, PreviousState& previous_state, const Event& event )
		{
#pragma warning(push)
#pragma warning(disable:4839)
			LeaveState( previous_state, event );
			TargetState& new_state = machine.template TransitionTo<TargetState>();
			auto enter_action = EnterState( new_state, event ); // we allow EnterState to return an action so we can have transient states
			enter_action.Execute( machine, new_state, event );
#pragma warning(pop)
		}

	private:
		// NOTE: using SFINAE to detect whether a OnEnter or OnLeave method was defined for a given transition type

		void LeaveState( ... ) {}
		auto EnterState( ... ) -> NoAction { return NoAction{}; }

		template<typename PreviousState, typename Event>
		auto LeaveState( PreviousState& previous_state, const Event& event ) -> decltype(previous_state.OnLeave( event )) { return previous_state.OnLeave( event ); }

		template<typename NewState, typename Event>
		auto EnterState( NewState& new_state, const Event& event ) -> decltype(new_state.OnEnter( event )) { return new_state.OnEnter( event ); }
	};
}
