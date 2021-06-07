#include "SpriteHelpers.hpp"

#include "CommonSpriteComponent.hpp"

namespace Game::Sprite::Helpers
{
	void SetSprite( ecs::EntityHandle& entity, std::string_view sprite_id, std::optional<ColourRGBA> blend )
	{
		ASSERT( !!entity );
		if (auto* sprite = entity.try_get<CommonSpriteComponent>())
		{
			sprite->id = std::string{ sprite_id };
			if (blend)
				sprite->blend = *blend;
		}
		else
			entity.emplace<CommonSpriteComponent>( CommonSpriteComponent{ .id = std::string{ sprite_id }, .blend = blend.value_or( 0xFFFFFFFF ) } );
	}

	void SetBlend( ecs::EntityHandle& entity, ColourRGBA blend )
	{
		ASSERT( !!entity );
		auto& sprite = entity.get<CommonSpriteComponent>();
		sprite.blend = blend;
	}

	void RemoveSprite( ecs::EntityHandle& entity )
	{
		ASSERT( !!entity );
		entity.remove_if_exists<CommonSpriteComponent>();
	}
}
