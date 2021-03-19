#pragma once

namespace StateMachine::Actions
{
	/// <summary>
	/// Informs that the state machine does not change from the event
	/// </summary>
	struct NoAction
	{
		template<typename Machine, typename State, typename Event>
		void Execute( Machine&, State&, const Event& )
		{
		}
	};
}
