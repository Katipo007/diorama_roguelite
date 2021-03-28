#pragma once

#include <utility>
#include <variant>

namespace fsm::Actions
{
	/// <summary>
	/// Helper mechanism to say we will perform one of these action types
	/// </summary>
	/// <typeparam name="...Actions">Types of action we might perform</typeparam>
	template<typename... ActionTypes>
	class OneOf
	{
	public:
		template<typename T>
		OneOf( T&& arg )
			: options( std::forward<T>( arg ) )
		{
		}

		template<typename Machine, typename State, typename Event>
		void Execute( Machine& machine, State& state, const Event& event )
		{
			std::visit( [&machine, &state, &event]( auto& action )
			{
				action.Execute( machine, state, event );
			}, options );
		}

	private:
		std::variant<ActionTypes...> options;
	};
}
