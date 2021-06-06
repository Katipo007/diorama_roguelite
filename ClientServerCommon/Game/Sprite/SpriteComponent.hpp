#pragma once

#include "Common/Core/Resources/ResourceHandle.hpp"

namespace Graphics { class Sprite; }

namespace Game::Sprite
{
	struct SpriteComponent
	{
		virtual ~SpriteComponent() = default;

		Resources::ResourceHandle<Graphics::Sprite> sprite;
	};
}
