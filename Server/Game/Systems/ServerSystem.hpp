#pragma once

#include "ClientServerCommon/ecs.hpp"
#include "Common/Utility/Timestep.hpp"

namespace yojimbo { class Server; }

namespace Game::Systems
{
	void ServerProcessIncoming( yojimbo::Server& server, ecs::Registry& registry, const PreciseTimestep& ts );
	void ServerProcessOutgoing( yojimbo::Server& server, ecs::Registry& registry, const PreciseTimestep& ts );
}
