#pragma once

#include "ClientServerCommon/ecs.hpp"
#include "ClientServerCommon/Networking.hpp"
#include "Common/Utility/Math/Vec3.hpp"

namespace Game::Helpers
{
	ecs::EntityHandle CreatePlayer( ecs::Registry& registry, Networking::ClientId id, glm::vec3 location );
}
