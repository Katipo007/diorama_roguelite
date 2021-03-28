#pragma once

namespace fsm
{
	/// <summary>
	/// Helper mechanism to save writing out handlers for every type of event
	/// </summary>
	/// <typeparam name="Action">Action to perform for otherwise unhandled events</typeparam>
	template<typename Action>
	struct DefaultAction
	{
		template <typename Event>
		Action HandleEvent( const Event& ) const { return Action{}; }
	};
}
