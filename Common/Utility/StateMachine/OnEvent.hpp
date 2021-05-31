#pragma once

#include "Concepts.hpp"

namespace fsm
{
	/// <summary>
	/// Helper mechanism perform basic actions on a given event
	/// </summary>
	/// <typeparam name="Event">Event to trigger this action on</typeparam>
	/// <typeparam name="Action">Action to perform</typeparam>
	template<Concepts::Event Event, Concepts::Action Action>
	struct OnEvent
	{
		Action HandleEvent( const Event& ) const { return Action{}; }
	};
}
