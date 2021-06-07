#pragma once

#include "ClientServerCommon/ecs.hpp"
#include "Common/Utility/Timestep.hpp"

namespace Game::Movement
{
	void TestSystem( ecs::Registry& registry, const PreciseTimestep& ts );
}
