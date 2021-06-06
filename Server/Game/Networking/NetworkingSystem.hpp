#pragma once

#include "ClientServerCommon/ecs.hpp"
#include "Common/Utility/Timestep.hpp"

namespace yojimbo { class Server; }

namespace Game::Networking
{
	void IncomingSystem( yojimbo::Server& server, ecs::Registry& registry, const PreciseTimestep& ts );
	void OutgoingSystem( yojimbo::Server& server, ecs::Registry& registry, const PreciseTimestep& ts );
}
