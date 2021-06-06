#pragma once

#include "ClientServerCommon/ecs.hpp"

#include "ClientServerCommon/Game/ClientSync/ClientSyncTypes.hpp"
#include "Common/DataTypes/Bytes.hpp"

namespace Game::Networking { struct ConnectionComponent; }

namespace Game::ClientSync::Helpers
{
	void SyncEntityToClient( ecs::EntityHandle to_sync, Networking::ConnectionComponent& client, const bool reliable = false );
	void RemoveEntityFromClient( ecs::Entity to_remove, Networking::ConnectionComponent& client );

	Bytes WriteComponents( const ecs::EntityConstHandle entity );
}
