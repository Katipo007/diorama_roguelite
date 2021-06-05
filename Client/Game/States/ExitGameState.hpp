#pragma once

#include "Events.hpp"
#include "Common/Utility/StateMachine/DefaultAction.hpp"
#include "Common/Utility/StateMachine/NoAction.hpp"
#include "Common/Utility/NonCopyable.hpp"

namespace Game::States
{
	class ExitGameState
		: public fsm::DefaultAction<fsm::NoAction>
		, NonCopyable
	{

	};
}
