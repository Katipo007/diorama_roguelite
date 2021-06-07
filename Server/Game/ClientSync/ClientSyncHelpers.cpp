#include "ClientSyncHelpers.hpp"

#include "ClientServerCommon/Game/ClientSync/ClientSyncMessages.hpp"
#include "ClientServerCommon/Game/ClientSync/SyncableComponents.hpp"
#include "Server/Game/Networking/NetworkingHelpers.hpp"
#include "Components/SyncableComponent.hpp"
#include "Components/SyncRecordComponent.hpp"
#include "Common/Utility/StringUtility.hpp"

namespace
{
	Game::ClientSync::EntitySyncId GetNextSyncId()
	{
		static Game::ClientSync::EntitySyncId id = 0;
		return ++id;
	}
}

namespace Game::ClientSync::Helpers
{
	namespace
	{
		template<SerialisableComponent C>
		inline void WriteComponent( C& component, Serialiser& serialiser, const bool include_identitier = true )
		{
			if (include_identitier)
			{
				static_assert(sizeof( ComponentIdentifiers::identifier_type ) == 4);
				serialiser.value4b( ComponentIdentifiers::template type<C> );
			}
			component.Serialise( serialiser );
		}

		template<SerialisableComponent C>
		bool WriteComponent( const ecs::EntityHandle entity, Serialiser& serialiser, const bool include_identifier = true )
		{
			ASSERT( !!entity );
			if (auto* component = entity.try_get<C>())
			{
				WriteComponent<C>( *component, serialiser, include_identifier );
				return true;
			}

			return false;
		}

		Buffer_T WriteComponents( const ecs::EntityHandle entity )
		{
			if (!entity)
				return {};

			Buffer_T data;
			Serialiser serialiser{ data };

			WriteComponent<Name::NameComponent>( entity, serialiser );

			serialiser.adapter().flush();
			const auto used_num_bytes = serialiser.adapter().writtenBytesCount();
			return { std::begin( data ), std::begin( data ) + used_num_bytes };
		}
	}

	void SyncEntityToClient( ecs::EntityHandle to_sync, Networking::ConnectionComponent& client, bool reliable )
	{
		if (!to_sync)
			return;

		// Check that this entity is syncable
		auto* sync_component = to_sync.try_get<SyncableComponent>();
		ASSERT( sync_component && sync_component->sync_id != 0 );
		if (!sync_component || sync_component->sync_id == 0)
			return;

		// Check what the client already knows about this entity
		auto& registry = *to_sync.registry();
		const auto client_entity = entt::to_entity( registry, client );
		ASSERT( registry.valid( client_entity ) );
		auto& sync_record = registry.get_or_emplace<SyncRecordComponent>( client_entity );
		
		// Client doesn't know about this entity, we need to inform the client about it before updating
		if (!sync_record.known_entities.contains( sync_component->sync_id ) )
		{
			reliable = true;

			const bool success = Networking::Helpers::SendMessage<Messages::ServerClientAddEntity>( client, Networking::ChannelType::Reliable, [&]( Messages::ServerClientAddEntity& msg )
				{
					msg.entity_sync_id = sync_component->sync_id;
					StringUtility::StringToArray( "PLACEHOLDER", msg.entity_type );
				} );

			if (success)
				sync_record.known_entities.emplace( sync_component->sync_id );
			else
				NOT_IMPLEMENTED;
		}
			
		// Serialise the component data
		const auto component_data = WriteComponents( to_sync );
		if (component_data.empty())
			return;

		// Send the message to client
		const auto message_initialiser = [&]( Messages::ServerClientEntitySync& msg )
		{
			msg.entity_sync_id = sync_component->sync_id;
		};
		Networking::Helpers::SendBlockMessage<Messages::ServerClientEntitySync>( client, reliable ? Networking::ChannelType::Reliable : Networking::ChannelType::Unreliable, message_initialiser, (uint8_t*)component_data.data(), std::size( component_data ) );
	}

	void MakeSerialisable( const ecs::EntityHandle entity )
	{
		auto& sync = entity.get_or_emplace<SyncableComponent>();
		if (sync.sync_id == 0)
			sync.sync_id = GetNextSyncId();

		Dirty( entity );
	}

	void Dirty( const ecs::EntityHandle entity, std::optional<ComponentIdentifiers::identifier_type> specific_component )
	{
		if (auto* sync = entity.try_get<SyncableComponent>())
		{
			if (specific_component)
				NOT_IMPLEMENTED;
			else if( !sync->dirty )
				sync->dirty = true;
		}
	}
}
