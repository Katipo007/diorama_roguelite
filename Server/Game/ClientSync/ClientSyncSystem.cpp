#include "ClientSyncSystem.hpp"

#include "Server/Game/Networking/Components/ActiveClientComponent.hpp"
#include "Server/Game/Networking/Components/ConnectionComponent.hpp"
#include "Server/Game/Networking/Components/PendingClientComponent.hpp"
#include "Components/SyncableComponent.hpp"
#include "Components/SyncRecordComponent.hpp"
#include "ClientSyncHelpers.hpp"

namespace Game::ClientSync
{
	void ClientSyncSystem( ecs::Registry& registry )
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
					const bool client_knows_of_this = client_sync_record.known_entities.contains( to_sync_component.sync_id );

					if (!client_knows_of_this)
					{
						auto& client_connection = registry.get<Networking::ConnectionComponent>( client_entity );
						Helpers::AddEntityToClient( to_sync, client_connection );
						client_sync_record.known_entities.emplace( to_sync_component.sync_id );
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
}
