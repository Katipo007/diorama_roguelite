#include "PlayerHelpers.hpp"

#include "ClientServerCommon/Game/Components/Name.hpp"
#include "ClientServerCommon/Game/Components/Sprite.hpp"
#include "ClientServerCommon/Game/Components/Transform.hpp"

namespace Game::Helpers
{
    ecs::EntityHandle CreatePlayer( ecs::Registry& registry, Networking::ClientId id, glm::vec3 location )
    {
        auto entity = registry.create();

        registry.emplace<Components::Name>( entity, "Unnamed" );
        registry.emplace<Components::Sprite>( entity );
        registry.emplace<Components::Transform>( entity, location );

        return { registry, entity };
    }
}
