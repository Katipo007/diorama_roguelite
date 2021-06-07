#pragma once

#include <unordered_set>
#include "ClientServerCommon/ecs.hpp"
#include "ClientServerCommon/Game/ClientSync/ClientSyncTypes.hpp"


namespace Game::ClientSync
{
	struct SyncableComponent final
	{
		EntitySyncId sync_id{ 0 };
		std::unordered_set<ecs::Entity> select; //< Should only be serialised to a select number of clients
		bool dirty = false; //< Whether this entity has become dirty and needs resynchronisation
	};
}
