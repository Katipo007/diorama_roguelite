#pragma once

#include "ClientServerCommon/ecs.hpp"
#include "Common/DataTypes/Vec3.hpp"

namespace Game::Player::Helpers
{
	bool ApplyMovementFromClient( const ecs::EntityHandle player_entity, const glm::vec3& client_specified_position );
}
