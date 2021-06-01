#pragma once

#include <type_traits>

#include "NoAction.hpp"
#include "Concepts.hpp"

namespace fsm
{
	namespace detail
	{
		template<typename T>
		concept HasDefaultOnEnterMethod = requires(T t)
		{
			{ t.OnEnter() };
		};

		template<typename T, typename... Args>
		concept HasOnEnterMethod = requires( T t )
		{
			{ t.OnEnter( std::declval<Args>()... ) };
		};

		template<typename T>
		concept HasDefaultOnLeaveMethod = requires(T t)
		{
			{ t.OnLeave() };
		};

		template<typename T, typename... Args>
		concept HasOnLeaveMethod = requires( T t )
		{
			{ t.OnLeave( std::declval<Args>()... ) };
		};
	}

	/// <summary>
	/// Have the state machine transition to the given state
	/// </summary>
	/// <typeparam name="TargetState">State type to transition to</typeparam>
	template<class TargetState>
	class TransitionTo
	{
	public:
		template<typename Machine, Concepts::State PreviousState, Concepts::Event Event>
		void Execute( Machine& machine, PreviousState& previous_state, const Event& event )
		{
			static_assert(Concepts::State<TargetState>); // we have this concept requirement inside the definition so that we can using in-complete types

#pragma warning(push)
#pragma warning(disable:4839)
			LeaveState( previous_state, event );
			TargetState& new_state = machine.template TransitionTo<TargetState>();
			Concepts::Action auto enter_action = EnterState( new_state, event ); // we allow EnterState to return an action so we can have transient states
			enter_action.Execute( machine, new_state, event );
#pragma warning(pop)
		}

	private:
		// NOTE: using SFINAE to detect whether a OnEnter or OnLeave method was defined for a given transition type

		void LeaveState( ... ) {}
		Concepts::Action auto EnterState( ... ) { return NoAction{}; }

		// version which doesn't take event
		template<Concepts::State NewState, Concepts::Event Event>
		Concepts::Action auto EnterState( NewState& new_state, const Event& ) requires(!detail::HasOnEnterMethod<NewState, const Event&> && detail::HasDefaultOnEnterMethod<NewState>) { return new_state.OnEnter(); }

		template<Concepts::State PreviousState, Concepts::Event Event>
		void LeaveState( PreviousState& previous_state, const Event& ) requires(!detail::HasOnLeaveMethod<PreviousState, const Event&> && detail::HasDefaultOnLeaveMethod<PreviousState>) { return previous_state.OnLeave(); }

		// version which does take event
		template<Concepts::State NewState, Concepts::Event Event>
		Concepts::Action auto EnterState( NewState& new_state, const Event& event ) requires(detail::HasOnEnterMethod<NewState, const Event&>) { return new_state.OnEnter( event ); }

		template<Concepts::State PreviousState, Concepts::Event Event>
		void LeaveState( PreviousState& previous_state, const Event& event ) requires(detail::HasOnLeaveMethod<PreviousState, const Event&>) { return previous_state.OnLeave( event ); }
	};
}
