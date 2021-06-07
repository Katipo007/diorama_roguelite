#include "SpriteSystem.hpp"

#include "ClientServerCommon/Game/Transform/Components/PositionComponent.hpp"
#include "ClientServerCommon/Game/Transform/TransformHelpers.hpp"
#include "SpriteComponent.hpp"
#include "Common/Core/ResourceManager.hpp"
#include "Visual/SpriteBatcher.hpp"

namespace Game::Sprite
{
	void ClientSystem( ecs::Registry& registry, ResourceManager& resources, Visual::SpriteBatcher& renderer )
	{
		registry.view<SpriteComponent, Transform::PositionComponent>().each( [&]( const ecs::Entity, SpriteComponent& sprite, const Transform::PositionComponent& position )
			{
				if (sprite.dirty)
				{
					sprite.cached_sprite_resource = resources.Get<Graphics::Sprite>( sprite.id );
					sprite.dirty = false;
				}

				renderer.DrawStandingSprite( sprite.cached_sprite_resource, Transform::Helpers::GetPosition( position ) );
			} );
	}
}
