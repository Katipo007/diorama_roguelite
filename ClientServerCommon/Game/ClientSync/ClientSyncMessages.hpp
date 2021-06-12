#pragma once

#include "ClientServerCommon/Game/Networking/Messages/MessageDefines.hpp"
#include "ClientSyncTypes.hpp"

namespace Game::ClientSync::Messages
{
#pragma warning(disable:4127)

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
		std::vector<EntitySyncId> entity_sync_ids;

		template<YojimboPlugin::Concepts::SerializeStream Stream>
		bool Serialize( Stream& stream )
		{
			uint32_t num_entries{ 0 };
			if (Stream::IsWriting)
			{
				ASSERT( std::size( entity_sync_ids ) < std::numeric_limits<uint32_t>::max() );
				num_entries = static_cast<uint32_t>(std::size( entity_sync_ids ));
				ASSERT( num_entries != 0 );
			}
			serialize_varint32( stream, num_entries );

			if (Stream::IsReading)
			{
				ASSERT( num_entries != 0 );
				entity_sync_ids.resize( num_entries );
			}
			serialize_bytes( stream, reinterpret_cast<uint8_t*>(entity_sync_ids.data()), static_cast<int>(sizeof( EntitySyncId ) * num_entries) );

			return true;
		}
	END_MESSAGE();

	BEGIN_MESSAGE( ServerClientEntityComponentRemoved )
		EntitySyncId entity_sync_id{ 0 };
		ComponentTypeMask removed_components;

		template<YojimboPlugin::Concepts::SerializeStream Stream>
		bool Serialize( Stream& stream )
		{
			serialize_varint64( stream, entity_sync_id );

			uint64_t value{ 0 };
			if constexpr (Stream::IsWriting)
				value = removed_components.to_ullong();
			serialize_uint64( stream, value );
			if constexpr (Stream::IsReading)
				removed_components = ComponentTypeMask{ value };
			return true;
		}
	END_MESSAGE();
}

#pragma warning(default:4127)

#include "ClientServerCommon/Game/Networking/Messages/MessageUndefines.hpp"
