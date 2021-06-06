#pragma once

#include "Common/Core/Resources/ResourceHandle.hpp"

namespace Graphics { class Sprite; }

namespace Game::Components
{
	struct Sprite
	{
		Resources::ResourceHandle<Graphics::Sprite> sprite;
	};
}
