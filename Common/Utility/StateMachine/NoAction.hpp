#pragma once

#include "Concepts.hpp"

namespace fsm
{
	/// <summary>
	/// Informs that the state machine does not change from the event
	/// </summary>
	struct NoAction
	{
		template<typename Machine, Concepts::State State, Concepts::Event Event>
		void Execute( Machine&, State&, const Event& )
		{
		}
	};
}
