#include "ClientSyncSystem.hpp"

#include "Server/Game/Networking/Components/ActiveClientComponent.hpp"
#include "Server/Game/Networking/Components/ConnectionComponent.hpp"
#include "Server/Game/Networking/Components/PendingClientComponent.hpp"
#include "Components/SyncableComponent.hpp"
#include "ClientSyncHelpers.hpp"

namespace Game::ClientSync
{
	void ClientSyncSystem( ecs::Registry& registry )
	{
		const auto client_syncables = registry.view<SyncableComponent>();
		const auto clients = registry.view<Networking::ConnectionComponent>( entt::exclude<Networking::PendingClientComponent> );

		client_syncables.each( [&]( const ecs::Entity entity, SyncableComponent& sync )
			{
				if (!sync.dirty)
					return;

				for (auto& client_entity : clients)
				{
					if (!sync.select.empty() && !sync.select.contains( client_entity ))
						continue;

					auto& client_connection = registry.get<Networking::ConnectionComponent>( entity );

					Helpers::SyncEntityToClient( ecs::EntityHandle{ registry, entity }, client_connection );
				}

				sync.dirty = false;
			} );
	}
}
