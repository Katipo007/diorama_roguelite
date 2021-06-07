#pragma once

#include "ClientServerCommon/Game/ClientSync/ClientSyncTypes.hpp"

namespace Game::ClientSync
{
	/// <summary>
	/// Stores information for which entities have been synced to this client
	/// </summary>
	struct SyncRecordComponent final // stupid name, needs renaming
	{
		std::unordered_set<ecs::Entity> known_entities;
	};
}
