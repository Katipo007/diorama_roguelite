#pragma once

namespace fsm
{
	/// <summary>
	/// Helper mechanism perform basic actions on a given event
	/// </summary>
	/// <typeparam name="Event">Event to trigger this action on</typeparam>
	/// <typeparam name="Action">Action to perform</typeparam>
	template<typename Event, typename Action>
	struct OnEvent
	{
		Action HandleEvent( const Event& ) const { return Action{}; }
	};
}
