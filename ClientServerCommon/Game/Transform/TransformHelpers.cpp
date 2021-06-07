#include "TransformHelpers.hpp"

#include "Components/PositionComponent.hpp"

namespace Game::Transform::Helpers
{
	void SetPosition( ecs::EntityHandle entity, glm::vec3 position )
	{
		entity.emplace_or_replace<PositionComponent>( PositionComponent{ .position = { position.x, position.y, position.z } } );
	}

	void Move( ecs::EntityHandle entity, glm::vec3 delta )
	{
		entity.patch<PositionComponent>( [&]( PositionComponent& pos )
			{
				pos.position[0] += delta.x;
				pos.position[1] += delta.y;
				pos.position[2] += delta.z;
			} );
	}

	glm::vec3 GetAbsolutePosition( const ecs::EntityConstHandle entity )
	{
		return GetPosition( entity.get<PositionComponent>() );
	}

	glm::vec3 GetPosition( const PositionComponent& position )
	{
		return glm::vec3{ position.position[0], position.position[1], position.position[2] };
	}
}
