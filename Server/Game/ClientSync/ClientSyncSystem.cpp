#include "ClientSyncSystem.hpp"

#include "Server/Game/Networking/ActiveClientComponent.hpp"
#include "Server/Game/Networking/ConnectionComponent.hpp"
#include "Server/Game/Networking/PendingClientComponent.hpp"
#include "ClientSyncComponent.hpp"
#include "ClientSyncHelpers.hpp"

namespace Game::ClientSync
{
	void ClientSyncSystem( ecs::Registry& registry )
	{
		const auto client_syncables = registry.view<ClientSyncComponent>();
		const auto clients = registry.view<Networking::ConnectionComponent>( entt::exclude<Networking::PendingClientComponent> );

		client_syncables.each( [&]( const ecs::Entity entity, ClientSyncComponent& sync )
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
