#pragma once

#include "ClientServerCommon/ecs.hpp"

#include "ClientServerCommon/Game/ClientSync/ClientSyncTypes.hpp"

namespace Game::Networking { struct ConnectionComponent; }

namespace Game::ClientSync::Helpers
{
	void MakeSerialisable( const ecs::EntityHandle entity );
	void Dirty( const ecs::EntityHandle entity, std::optional<ComponentTypeId> specific_component = std::nullopt );

	void SyncEntityToClient( ecs::EntityHandle to_sync, Networking::ConnectionComponent& client, const bool reliable = false );
	void RemoveEntityFromClient( ecs::Entity to_remove, Networking::ConnectionComponent& client );
}
