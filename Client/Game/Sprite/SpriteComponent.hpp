#pragma once

#include "ClientServerCommon/Game/Sprite/CommonSpriteComponent.hpp"
#include "Common/Core/Resources/ResourceHandle.hpp"
#include "Visual/Resources/SpriteSheet.hpp"

namespace Game::Sprite
{
	struct SpriteComponent final
		: public CommonSpriteComponent
	{
		Resources::ResourceHandle<Graphics::Sprite> cached_sprite_resource;
		bool dirty = false;

		void Serialise( typename auto& serialiser )
		{
			CommonSpriteComponent::Serialise( serialiser );
			dirty = true;
		}
	};
}
