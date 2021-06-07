#pragma once

#include "ClientServerCommon/Game/Networking/Messages/MessageDefines.hpp"
#include "ClientSyncTypes.hpp"

namespace Game::ClientSync::Messages
{
	BEGIN_MESSAGE( ServerClientAddEntity )
		EntitySyncId entity_sync_id{ 0 };
		std::array<char, 128> entity_type{ 0 };
		std::array<float, 3> position;

		template<YojimboPlugin::Concepts::SerializeStream Stream>
		bool Serialize( Stream& stream )
		{
			serialize_varint64( stream, entity_sync_id );
			serialize_string( stream, entity_type.data(), (int)std::size( entity_type ) );
			serialize_bytes( stream, (uint8_t*)position.data(), (int)(std::size( position ) * sizeof( position[0] )) );
			return true;
		}
	END_MESSAGE();

	BEGIN_BLOCK_MESSAGE( ServerClientEntitySync )
		EntitySyncId entity_sync_id{ 0 };
		
		template<YojimboPlugin::Concepts::SerializeStream Stream>
		bool Serialize( Stream& stream )
		{
			serialize_varint64( stream, entity_sync_id );
			return true;
		}
	END_BLOCK_MESSAGE();

	BEGIN_MESSAGE( ServerClientRemoveEntity )
		EntitySyncId entity_sync_id{ 0 };

		template<YojimboPlugin::Concepts::SerializeStream Stream>
		bool Serialize( Stream& stream )
		{
			serialize_varint64( stream, entity_sync_id );
			return true;
		}
	END_MESSAGE();
}

#include "ClientServerCommon/Game/Networking/Messages/MessageUndefines.hpp"
