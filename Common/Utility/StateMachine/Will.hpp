#pragma once

namespace fsm
{
	/// <summary>
	/// Helper mechanism to save having to pull in handlers from base class mechanisms.
	/// i.e. prevents having to manually specifiy `using DefaultAction::HandleEvent;` etc for all mechanisms
	/// </summary>
	/// <typeparam name="...Handlers"></typeparam>
	template<typename... Handlers>
	struct Will : public Handlers...
	{
		using Handlers::HandleEvent...;
	};
}
