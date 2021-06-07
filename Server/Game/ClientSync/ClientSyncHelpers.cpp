#include "ClientSyncHelpers.hpp"

#include "ClientServerCommon/Game/ClientSync/ClientSyncMessages.hpp"
#include "ClientServerCommon/Game/ClientSync/SyncableComponents.hpp"
#include "Server/Game/Networking/NetworkingHelpers.hpp"
#include "Components/SyncableComponent.hpp"
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

		Buffer_T WriteComponents( const ecs::EntityHandle entity, ComponentTypeMask component_mask )
		{
			if (!entity)
				return {};

			Buffer_T data;
			Serialiser serialiser{ data };

			//
			// Add components which should be serialised here
			//

			if (component_mask.all() || component_mask.none())
			{
#pragma push_macro("X")
#define X( COMPONENT ) WriteComponent<COMPONENT>( entity, serialiser );
				CLIENT_SYNCABLE_COMPONENTS
#pragma pop_macro("X")
			}
			else
			{
#pragma push_macro("X")
#define X( COMPONENT ) if( component_mask.test( ComponentIdentifiers::template type<COMPONENT> ) ) WriteComponent<COMPONENT>( entity, serialiser );
				CLIENT_SYNCABLE_COMPONENTS
#pragma pop_macro("X")
			}

			//
			// end serialising components
			//

			serialiser.adapter().flush();
			const auto used_num_bytes = serialiser.adapter().writtenBytesCount();
			return { std::begin( data ), std::begin( data ) + used_num_bytes };
		}
	}

	void AddEntityToClient( const ecs::EntityHandle to_add, Networking::ConnectionComponent& client )
	{
		if (!to_add)
			return;

		if (auto* syncable = to_add.try_get<SyncableComponent>())
		{
			ASSERT( syncable->sync_id > 0 );
			Networking::Helpers::SendMessage<Messages::ServerClientAddEntity>( client, Networking::ChannelType::Reliable, [&]( Messages::ServerClientAddEntity& msg )
				{
					msg.entity_sync_id = syncable->sync_id;
					StringUtility::StringToArray( "PLACEHOLD", msg.entity_type );
				} );
		}
	}

	void SyncEntityToClient( ecs::EntityHandle to_sync, Networking::ConnectionComponent& client, ComponentTypeMask component_mask, const bool force_reliable )
	{
		static const thread_local ComponentTypeMask unreliable_mask = ComponentTypeMask{}.set( ComponentIdentifiers::type<Transform::PositionComponent> );

		if (!to_sync)
			return;

		if (auto* syncable = to_sync.try_get<SyncableComponent>())
		{
			ASSERT( syncable->sync_id > 0 );

			// Serialise the component data
			const auto component_data = WriteComponents( to_sync, component_mask );
			if (component_data.empty())
				return;

			const bool reliable = force_reliable || ((component_mask & unreliable_mask) != component_mask);
			if (reliable)
				LOG_INFO( LoggingChannels::Server, "Did reliable entity sync of '{}' to client ({})", syncable->sync_id, client.client_index);

			Networking::Helpers::SendBlockMessage<Messages::ServerClientEntitySync>(
				client
				, reliable ? Networking::ChannelType::Reliable : Networking::ChannelType::Unreliable
				, [&]( Messages::ServerClientEntitySync& msg ) { msg.entity_sync_id = syncable->sync_id; }
				, component_data.data(), std::size( component_data )
				);
		}
	}

	void RemoveEntityFromClient( const ecs::EntityHandle to_remove, Networking::ConnectionComponent& client )
	{
		if (!to_remove)
			return;

		if (auto* syncable = to_remove.try_get<SyncableComponent>())
		{
			ASSERT( syncable->sync_id > 0 );
			Networking::Helpers::SendMessage<Messages::ServerClientRemoveEntity>( client, Networking::ChannelType::Reliable, [&]( Messages::ServerClientRemoveEntity& msg )
				{
					msg.entity_sync_id = syncable->sync_id;
				} );
		}
	}

	void RemoveComponentFromClient( const ecs::EntityConstHandle entity, ComponentTypeId component_type, Networking::ConnectionComponent& client )
	{
		if (!entity)
			return;

		if (auto* syncable = entity.try_get<SyncableComponent>())
		{
			ASSERT( syncable->sync_id > 0 );
			Networking::Helpers::SendMessage<Messages::ServerClientEntityComponentRemoved>( client, Networking::ChannelType::Reliable, [&]( Messages::ServerClientEntityComponentRemoved& msg )
				{
					msg.entity_sync_id = syncable->sync_id;
					msg.component_type = component_type;
				} );
		}
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
				sync->dirty_components.set( *specific_component );
			else
				sync->dirty_components.set(); // set all
		}
	}
}
