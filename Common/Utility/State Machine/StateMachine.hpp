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

	namespace detail
	{
		template<class... B>
		using or_ = std::disjunction<B...>;

		template <typename T, typename Tuple>
		struct has_type;

		template <typename T, typename... Us>
		struct has_type<T, std::tuple<Us...>> : or_<std::is_same<T, Us>...> {};
	}

	template<typename... _States, typename... _Events>
	class Machine<States<_States...>, Events<_Events...>>
	{
		using Events_T = std::tuple<_Events...>;

	public:
		Machine()
			: states()
			, current_state( &std::get<0>( states ) )
		{}

		virtual ~Machine() noexcept {}

		// contruct the machine with pre-created states
		Machine( _States&&... states_ )
			: states( std::forward<_States>( states_ )... )
			, current_state( &std::get<0>( states ) )
		{
		}

		// We don't support copy constructors yet :(
		Machine( const Machine& ) = delete;
		// We don't support move constructors yet :(
		Machine( Machine&& ) = delete;
		// We don't support copy constructors yet :(
		Machine operator=( const Machine& ) = delete;
		// We don't support move constructors yet :(
		Machine operator=( Machine&& ) = delete;

		template<typename Event>
		void Handle( const Event& event )
		{
			static_assert( detail::has_type<Event, Events_T>::value, "Unhandled event type" );
			HandleBy( event, *this );
		}

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

		template<typename State>
		bool IsInState() const
		{
			return std::get_if<State*>( &current_state ) != nullptr;
		}

		template<typename State>
		State& TransitionTo()
		{
			auto& new_state = std::get<State>( states );
			current_state = &new_state;
			return new_state;
		}

	private:
		std::tuple<_States...> states;
		std::variant<_States*...> current_state; // start in the first listed event
	};
}
