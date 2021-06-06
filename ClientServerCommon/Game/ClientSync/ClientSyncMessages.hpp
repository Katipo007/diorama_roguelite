#pragma once

#include "ClientServerCommon/Game/Networking/Messages/MessageDefines.hpp"
#include "ClientSyncTypes.hpp"

namespace Game::ClientSync::Messages
{
	BEGIN_BLOCK_MESSAGE( ServerClientEntitySync )
		EntitySyncId entity_sync_id{ 0 };
		
		template<YojimboPlugin::Concepts::SerializeStream Stream>
		bool Serialize( Stream& stream )
		{
			serialize_varint64( stream, entity_sync_id );
			return true;
		}
	END_BLOCK_MESSAGE();
}

#include "ClientServerCommon/Game/Networking/Messages/MessageUndefines.hpp"
