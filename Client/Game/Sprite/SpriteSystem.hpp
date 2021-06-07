#pragma once

#include "ClientServerCommon/ecs.hpp"

class ResourceManager;
namespace Visual { class SpriteBatcher; }

namespace Game::Sprite
{
	void ClientSystem( ecs::Registry&, ResourceManager& resources, Visual::SpriteBatcher& renderer );
}
