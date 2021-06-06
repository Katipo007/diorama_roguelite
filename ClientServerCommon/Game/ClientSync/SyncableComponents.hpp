#pragma once

#include "ClientServerCommon/Game/Name/NameComponent.hpp"
#include "ClientServerCommon/Game/Sprite/SpriteComponent.hpp"

#include "ClientServerCommon/ecs.hpp"

namespace Game::ClientSync
{
	using ComponentIdentifiers = entt::identifier<
		Name::NameComponent,
		Sprite::SpriteComponent
	>;
}
