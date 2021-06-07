#pragma once

#include "ClientServerCommon/ecs.hpp"
#include "Common/DataTypes/Vec3.hpp"

namespace Game::Transform { struct PositionComponent; }

namespace Game::Transform::Helpers
{
	void SetPosition( ecs::EntityHandle entity, glm::vec3 position );
	void Move( ecs::EntityHandle entity, glm::vec3 delta );
	void RemovePosition( ecs::EntityHandle entity );

	glm::vec3 GetAbsolutePosition( const ecs::EntityConstHandle entity );
	glm::vec3 GetPosition( const PositionComponent& position );
}
