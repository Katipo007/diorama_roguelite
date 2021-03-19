#pragma once

/// <summary>
/// Draws heavily from the following articles (some chunks are a direct translation)
/// https://sii.pl/blog/implementing-a-state-machine-in-c17/
/// https://sii.pl/blog/implementing-a-state-machine-in-c17-part-2/
/// https://sii.pl/blog/implementing-a-state-machine-in-c17-part-3-compile-time-strings/
/// 
/// All the code is also avaliable in a repo here:
/// https://github.com/AdamsPL/state-machine
/// </summary>

#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>

namespace StateMachine
{
	template<typename... _States>
	class States;

	template<typename... _Events>
	class Events;

	template<class States, class Events>
	class Machine;

	namespace Actions
	{
		template<typename State>
		class TransitionTo;
	}

	namespace detail
	{
		template<class... B>
		using or_ = std::disjunction<B...>;

		template <typename T, typename Tuple>
		struct has_type;

		template <typename T, typename... Us>
		struct has_type<T, std::tuple<Us...>> : or_<std::is_same<T, Us>...> {};


		template <class T, class Tuple>
		struct index
		{
			static_assert(!std::is_same_v<Tuple, std::tuple<>>, "Could not find `T` in given `Tuple`");
		};

		template <class T, class... Types>
		struct index<T, std::tuple<T, Types...>>
		{
			static const std::size_t value = 0;
		};

		template <class T, class U, class... Types>
		struct index<T, std::tuple<U, Types...>>
		{
			static const std::size_t value = 1 + index<T, std::tuple<Types...>>::value;
		};

		template<typename T, typename... Ts>
		struct contains : std::bool_constant < (std::is_same<T, Ts>{} || ...) > {};
	}

	template<typename... _States, typename... _Events>
	class Machine<States<_States...>, Events<_Events...>>
	{
		// Allow TransitionTo action to access protected members so it can call TransitionTo method
		template<typename>
		friend class Actions::TransitionTo;

		using Events_T = std::tuple<_Events...>;
	public:
		Machine() : states(), current_state( &std::get<0>( states ) ) {}
		virtual ~Machine() noexcept {}

		// contruct the machine with pre-created states
		Machine( _States&&... states_ ) : states( std::forward<_States>( states_ )... ) , current_state( &std::get<0>( states ) ) {}

		// We don't support copy constructors yet :(
		Machine( const Machine& ) = delete;
		// We don't support move constructors yet :(
		Machine( Machine&& ) = delete;
		// We don't support copy constructors yet :(
		Machine operator=( const Machine& ) = delete;
		// We don't support move constructors yet :(
		Machine operator=( Machine&& ) = delete;

		/// <summary>
		/// Get the current state variant
		/// </summary>
		std::variant<_States*...> GetActiveState() { return current_state; }

		/// <summary>
		/// Test whether the machine is currently in a given state
		/// </summary>
		template<typename State>
		bool IsInState() const
		{
			static_assert(detail::contains<State, _States...>(), "StateMachine doesn't contain state State");

			return std::get_if<State*>( &current_state ) != nullptr;
		}

		/// <summary>
		/// Get a reference to a state, even if it isn't the current state
		/// </summary>
		template<typename State>
		State& GetState()
		{
			static_assert(detail::contains<State, _States...>(), "StateMachine doesn't contain state State");

			constexpr auto state_index = detail::index<State, decltype(states)>::value;
			return std::get<state_index>( states );
		}

		/// <summary>
		/// Get a const reference to a state, even if it isn't the current state
		/// </summary>
		template<typename State>
		const State& GetState() const
		{
			static_assert(detail::contains<State, _States...>(), "StateMachine doesn't contain state State");

			constexpr auto state_index = detail::index<State, decltype(states)>::value;
			return std::get<state_index>( states );
		}

		/// <summary>
		/// Have the current state handle an event
		/// </summary>
		template<typename Event>
		void Handle( const Event& event )
		{
			static_assert( detail::has_type<Event, Events_T>::value, "Unhandled event type" );
			HandleBy( event, *this );
		}

	protected:
		/// <summary>
		/// Have the current state of this machine handle an event, performing the action on a given machine
		/// </summary>
		template<typename Event>
		void HandleBy( const Event& event, Machine& machine )
		{
			auto PassEventToState = [&machine, &event]( auto state )
			{
				auto action = state->HandleEvent( event );
				action.Execute( machine, *state, event );
			};
			std::visit( PassEventToState, current_state );
		}

		/// <summary>
		/// IMPORTANT: DOES NOT CALL OnEnter or OnLeave on the new/old state!
		/// Have the machine transition to the given state.
		/// </summary>
		/// <returns></returns>
		template<typename State>
		State& TransitionTo()
		{
			auto& new_state = std::get<State>( states );
			current_state = &new_state;
			return new_state;
		}

	protected:
		std::tuple<_States...> states;
		std::variant<_States*...> current_state; // start in the first listed event
	};
}
