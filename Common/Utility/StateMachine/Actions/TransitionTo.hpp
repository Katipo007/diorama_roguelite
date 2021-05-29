#pragma once

#include <type_traits>
#include "NoAction.hpp"

namespace fsm::Actions
{
	namespace detail
	{
		// Primary template with a static assertion
		// for a meaningful error message
		// if it ever gets instantiated.
		// We could leave it undefined if we didn't care.

		template<typename, typename T>
		struct has_OnEnter {
			static_assert(
				std::integral_constant<T, false>::value,
				"Second template parameter needs to be of function type.");
		};

		template<typename, typename T>
		struct has_OnLeave {
			static_assert(
				std::integral_constant<T, false>::value,
				"Second template parameter needs to be of function type.");
		};

		// specialization that does the checking

		template<typename C, typename... Args>
		struct has_OnEnter<C, Args... > {
		private:
			template<typename T>
			static constexpr auto check( T* )
				-> typename
				std::is_object<
						decltype(std::declval<T>().OnEnter( std::declval<Args>()... ))
				>::type;  // attempt to call it and see if the return type is correct

			template<typename>
			static constexpr std::false_type check( ... );

			typedef decltype(check<C>( 0 )) type;

		public:
			static constexpr bool value = type::value;
		};

		template<typename C, typename... Args>
		struct has_OnLeave<C, Args... > {
		private:
			template<typename T>
			static constexpr auto check( T* )
				-> typename
				std::is_object<
				decltype(std::declval<T>().OnLeave( std::declval<Args>()... ))
				>::type;  // attempt to call it and see if the return type is correct

			template<typename>
			static constexpr std::false_type check( ... );

			typedef decltype(check<C>( 0 )) type;

		public:
			static constexpr bool value = type::value;
		};
	}

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
			static_assert(std::is_trivially_copyable<Event>::value, "Events must be trivially copyable");
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

		// version which doesn't take event
		template<typename NewState, typename Event, typename = typename std::enable_if<!detail::has_OnEnter<TargetState, const Event&>::value>::type>
		auto EnterState( NewState& new_state, const Event& ) -> decltype(new_state.OnEnter()) { return new_state.OnEnter(); }

		template<typename PreviousState, typename Event, typename = typename std::enable_if<!detail::has_OnLeave<TargetState, const Event&>::value>::type>
		auto LeaveState( PreviousState& previous_state, const Event& ) -> decltype(previous_state.OnLeave()) { return previous_state.OnLeave(); }

		// version which does take event
		template<typename NewState, typename Event, typename = typename std::enable_if<detail::has_OnEnter<TargetState, const Event&>::value>::type>
		auto EnterState( NewState& new_state, const Event& event ) -> decltype(new_state.OnEnter( event )) { return new_state.OnEnter( event ); }

		template<typename PreviousState, typename Event, typename = typename std::enable_if<detail::has_OnLeave<TargetState, const Event&>::value>::type>
		auto LeaveState( PreviousState& previous_state, const Event& event ) -> decltype(previous_state.OnLeave( event )) { return previous_state.OnLeave( event ); }
	};
}
