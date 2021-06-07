#pragma once

#include "ClientServerCommon/ecs.hpp"

#include "ClientServerCommon/Game/ClientSync/ClientSyncTypes.hpp"

namespace Game::Networking { struct ConnectionComponent; }
namespace Game::ClientSync { struct SyncRecordComponent; }

namespace Game::ClientSync::Helpers
{
	void MakeSerialisable( const ecs::EntityHandle entity );
	void Dirty( const ecs::EntityHandle entity, std::optional<ComponentTypeId> specific_component = std::nullopt );

	void AddEntityToClient( const ecs::EntityHandle to_add, Networking::ConnectionComponent& client );
	void SyncEntityToClient( const ecs::EntityHandle to_sync, Networking::ConnectionComponent& client );
	void RemoveEntityFromClient( const ecs::EntityHandle to_remove, Networking::ConnectionComponent& client );
}
