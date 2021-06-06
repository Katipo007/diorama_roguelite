#pragma once

#include "NoAction.hpp"
#include "OneOf.hpp"

#include "Concepts.hpp"

namespace fsm
{
	/// <summary>
	/// Helper mechanism to say we might perform an action, otherwise NoAction.
	/// </summary>
	/// <typeparam name="Action">Type of action we might perform</typeparam>
	template<Concepts::Action... Action>
	struct Might
		: public OneOf<Action..., NoAction>
	{
		constexpr Might() = default;

		using OneOf<Action..., NoAction>::OneOf;
	};
}
