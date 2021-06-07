#pragma once

#include <bitset>
#include "ClientServerCommon/ecs.hpp"
#include "ClientServerCommon/Game/ClientSync/ClientSyncTypes.hpp"


namespace Game::ClientSync
{
	struct SyncableComponent final
	{
		EntitySyncId sync_id{ 0 };
		ComponentTypeMask dirty_components;
	};
}
