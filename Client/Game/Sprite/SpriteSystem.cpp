#include "SpriteSystem.hpp"

#include "SpriteComponent.hpp"
#include "Common/Core/ResourceManager.hpp"
#include "Visual/SpriteBatcher.hpp"

namespace Game::Sprite
{
	void ClientSystem( ecs::Registry& registry, ResourceManager& resources, Visual::SpriteBatcher& renderer )
	{
		registry.view<SpriteComponent>().each( [&]( const ecs::Entity, SpriteComponent& sprite )
			{
				if (sprite.dirty)
				{
					sprite.cached_sprite_resource = resources.Get<Graphics::Sprite>( sprite.id );
					sprite.dirty = false;
				}

				renderer.DrawStandingSprite( sprite.cached_sprite_resource, { 0, 0, 0 } ); // TODO: positions
			} );
	}
}
