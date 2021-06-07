#include "TransformHelpers.hpp"

#include "Components/PositionComponent.hpp"

namespace Game::Transform::Helpers
{
	void SetPosition( ecs::EntityHandle entity, glm::vec3 position )
	{
		entity.emplace_or_replace<PositionComponent>( position );
	}

	void Move( ecs::EntityHandle entity, glm::vec3 delta )
	{
		entity.patch<PositionComponent>( [&]( PositionComponent& pos ) { pos.position += delta; } );
	}

	glm::vec3 GetAbsolutePosition( const ecs::EntityConstHandle entity )
	{
		return GetPosition( entity.get<PositionComponent>() );
	}

	glm::vec3 GetPosition( const PositionComponent& position )
	{
		return position.position;
	}
}
