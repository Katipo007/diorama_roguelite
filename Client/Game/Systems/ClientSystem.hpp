#pragma once

#include "ClientServerCommon/ecs.hpp"
#include "Common/Utility/Timestep.hpp"

namespace yojimbo { class Client; }

namespace Game::Systems
{
	void ClientProcessIncoming( yojimbo::Client& client, ecs::Registry& registry, const PreciseTimestep& ts );
	void ClientProcessOutgoing( yojimbo::Client& client, ecs::Registry& registry, const PreciseTimestep& ts );
}
