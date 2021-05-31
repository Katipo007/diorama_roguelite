#pragma once

#include "Concepts.hpp"

namespace fsm
{
	/// <summary>
	/// Helper mechanism to save writing out handlers for every type of event
	/// </summary>
	/// <typeparam name="Action">Action to perform for otherwise unhandled events</typeparam>
	template<Concepts::Action Action>
	struct DefaultAction
	{
		template <Concepts::Event Event>
		Action HandleEvent( const Event& ) const { return Action{}; }
	};
}
