#include "ClientSyncSystem.hpp"

#include "Server/Game/Networking/Components/ActiveClientComponent.hpp"
#include "Server/Game/Networking/Components/ConnectionComponent.hpp"
#include "Server/Game/Networking/Components/PendingClientComponent.hpp"
#include "ClientServerCommon/Game/ClientSync/SyncableComponents.hpp"
#include "Components/SyncableComponent.hpp"
#include "Components/SyncRecordComponent.hpp"
#include "ClientSyncHelpers.hpp"

namespace Game::ClientSync
{
	namespace
	{
		void OnSyncableDestroyed( ecs::Registry& registry, ecs::Entity entity )
		{
			const auto& syncable = registry.get<SyncableComponent>( entity );
			if (syncable.sync_id <= 0)
				return;

			const ecs::EntityHandle handle{ registry, entity };

			registry.view<SyncRecordComponent>().each( [&]( const ecs::Entity client_entity, SyncRecordComponent& record )
				{
					if (record.known_entities.contains( entity ))
					{
						if (auto* client_connection = registry.try_get<Networking::ConnectionComponent>( client_entity ))
							Helpers::RemoveEntityFromClient( handle, *client_connection );

						record.known_entities.erase( entity );
					}
				} );
		}

		void DirtyEntity( ecs::Registry& registry, ecs::Entity entity )
		{
			if (auto* syncable = registry.try_get<SyncableComponent>( entity ))
				syncable->dirty = true;
		}
	}

	void System( ecs::Registry& registry )
	{
		const auto client_syncables = registry.view<SyncableComponent>();
		const auto clients = registry.view<Networking::ConnectionComponent>( entt::exclude<Networking::PendingClientComponent> );

		client_syncables.each( [&]( const ecs::Entity syncable_entity, SyncableComponent& to_sync_component )
			{
				const auto to_sync = ecs::EntityHandle{ registry, syncable_entity };

				for (auto& client_entity : clients)
				{
					if (!to_sync_component.select.empty() && !to_sync_component.select.contains( client_entity ))
						continue;

					auto& client_sync_record = registry.get_or_emplace<SyncRecordComponent>( client_entity );
					const bool client_knows_of_this = client_sync_record.known_entities.contains( syncable_entity );

					if (!client_knows_of_this)
					{
						auto& client_connection = registry.get<Networking::ConnectionComponent>( client_entity );
						Helpers::AddEntityToClient( to_sync, client_connection );
						client_sync_record.known_entities.emplace( syncable_entity );
					}
					
					if (to_sync_component.dirty || !client_knows_of_this)
					{
						auto& client_connection = registry.get<Networking::ConnectionComponent>( client_entity );
						Helpers::SyncEntityToClient( to_sync, client_connection );
					}
				}

				to_sync_component.dirty = false;
			} );
	}

	void AttachHandlers( ecs::Registry& registry )
	{
		registry.on_destroy<SyncableComponent>().connect<&OnSyncableDestroyed>();

#pragma push_macro("X")
#define X( COMPONENT ) \
		registry.on_construct<COMPONENT>().connect<&DirtyEntity>(); \
		registry.on_update<COMPONENT>().connect<&DirtyEntity>();

		CLIENT_SYNCABLE_COMPONENTS
		
#pragma pop_macro("X")
	}

	void DetatchHandlers( ecs::Registry& registry )
	{
		registry.on_destroy<SyncableComponent>().disconnect<&OnSyncableDestroyed>();

#pragma push_macro("X")
#define X( COMPONENT ) \
		registry.on_construct<COMPONENT>().disconnect<&DirtyEntity>(); \
		registry.on_update<COMPONENT>().disconnect<&DirtyEntity>();

		CLIENT_SYNCABLE_COMPONENTS

#pragma pop_macro("X")
	}
}
