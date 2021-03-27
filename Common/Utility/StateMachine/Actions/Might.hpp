#pragma once

#include "NoAction.hpp"
#include "OneOf.hpp"

namespace StateMachine::Actions
{
	/// <summary>
	/// Helper mechanism to say we might perform an action, otherwise NoAction.
	/// </summary>
	/// <typeparam name="Action">Type of action we might perform</typeparam>
	template<typename... Action>
	struct Might
		: public OneOf<Action..., NoAction>
	{
		using OneOf<Action..., NoAction>::OneOf;
	};
}
