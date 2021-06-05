#pragma once

#include <utility>
#include <variant>

#include "Concepts.hpp"

namespace fsm
{
	/// <summary>
	/// Helper mechanism to say we will perform one of these action types
	/// </summary>
	/// <typeparam name="...Actions">Types of action we might perform</typeparam>
	template<Concepts::Action... ActionTypes>
	class OneOf
	{
	public:
		constexpr OneOf() = default;

		template<Concepts::Action T>
		OneOf( T&& arg )
			: options( std::forward<T>( arg ) )
		{
		}

		template<typename Machine, Concepts::State State, Concepts::Event Event>
		void Execute( Machine& machine, State& state, const Event& event )
		{
			std::visit( [&machine, &state, &event]( Concepts::Action auto& action )
			{
				action.Execute( machine, state, event );
			}, options );
		}

	private:
		std::variant<ActionTypes...> options;
	};
}
