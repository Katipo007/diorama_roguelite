#pragma once

#include "ClientServerCommon/Game/Name/NameComponent.hpp"
#include "ClientServerCommon/Game/Sprite/CommonSpriteComponent.hpp"
#include "ClientServerCommon/Game/Transform/Components/PositionComponent.hpp"

#include "ClientServerCommon/ecs.hpp"

#define CLIENT_SYNCABLE_COMPONENTS \
	X( ::Game::Name::NameComponent ) \
	X( ::Game::Sprite::CommonSpriteComponent ) \
	X( ::Game::Transform::PositionComponent )

namespace Game::ClientSync
{
	namespace detail { struct LastComponent { bool dummy; }; }

#pragma push_macro("X")
#define X( COMPONENT ) COMPONENT,
	using ComponentIdentifiers = entt::identifier<
		CLIENT_SYNCABLE_COMPONENTS
		detail::LastComponent
	>;
#pragma pop_macro("X")
}
