#pragma once

#include "ClientServerCommon/Game/ClientSync/ClientSyncTypes.hpp"
#include "ClientServerCommon/ecs.hpp"
#include "ClientServerCommon/Game/Networking/Channels.hpp"

namespace Game::ClientSync::Messages
{
	struct ServerClientAddEntity;
	struct ServerClientEntitySync;
	struct ServerClientRemoveEntity;
}

namespace Game::ClientSync::Helpers
{
	bool HandleMessage( ecs::Registry& registry, SyncedEntityMap_T& map, const Messages::ServerClientAddEntity& msg );
	bool HandleMessage( SyncedEntityMap_T& map, const Messages::ServerClientEntitySync& msg, const Networking::ChannelType channel );
	bool HandleMessage( SyncedEntityMap_T& map, const Messages::ServerClientRemoveEntity& msg );
}
