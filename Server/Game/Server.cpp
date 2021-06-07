#include "Server.hpp"

#include "ClientServerCommon/Game/Networking/Config.hpp"
#include "Server/Game/Networking/Components/ConnectionComponent.hpp"
#include "Server/Game/Networking/Components/PendingClientComponent.hpp"
#include "Server/Game/Networking/Constants.hpp"
#include "Server/Game/Networking/NetworkingSystem.hpp"
#include "Server/Game/ClientSync/ClientSyncSystem.hpp"

#include "Common/Utility/Containers.hpp"
#include "Common/Utility/MagicEnum.hpp"
#include "Common/Utility/StringUtility.hpp"

namespace Game
{
	Server::Server( Core& core, yojimbo::Address host_address )
		: core( core )
		, server( yojimbo::GetDefaultAllocator(), Networking::DefaultPrivateKey.data(), host_address, Networking::MakeConfiguration(), network_adapter, 0.0 )
		, network_adapter()
		, registry()
	{
		// Setup the networking server
		network_adapter.ServerClientConnected.connect( &Server::ClientConnectedHandler, this );
		network_adapter.ServerClientDisconnected.connect( &Server::ClientDisconnectedHandler, this );
		client_id_index_mapping.reserve( Networking::MaxServerClients );
		client_entities.reserve( Networking::MaxServerClients );

		ClientSync::AttachHandlers( registry );
	}

	Server::~Server()
	{
		if (server.IsRunning())
			server.Stop();
	}

	ecs::EntityHandle Server::GetClientByIndex( Networking::ClientIndex client_index )
	{
		if (const auto found_it = client_entities.find( client_index ); found_it != std::end( client_entities ))
			return found_it->second;

		return ecs::EntityHandle{};
	}

	ecs::EntityHandle Server::GetClientById( const Networking::ClientId& client_id )
	{
		if (const auto found_it = client_id_index_mapping.find( client_id ); found_it != std::end( client_id_index_mapping ))
			return GetClientByIndex( found_it->second );

		return ecs::EntityHandle{};
	}

	void Server::OnFixedUpdate( const PreciseTimestep& ts )
	{
		Networking::IncomingSystem( server, registry, ts );
		TickSimulation( ts );
		ClientSync::System( registry );
		Networking::OutgoingSystem( server, registry, ts );
	}

	void Server::OnVariableUpdate( const PreciseTimestep& )
	{
	}

	void Server::ClientConnectedHandler( Networking::Adapter&, Networking::ClientIndex index )
	{
		LOG_INFO( LoggingChannels::Server, "Client ({}) connected", index );

		const auto client_id = server.GetClientId( index );

		const auto entity = registry.create();
		registry.emplace<Networking::ConnectionComponent>( entity, server, index );
		registry.emplace<Networking::PendingClientComponent>( entity );

		client_entities[index] = ecs::EntityHandle( registry, entity );
		client_id_index_mapping[client_id] = index;
	}

	void Server::ClientDisconnectedHandler( Networking::Adapter&, Networking::ClientIndex index )
	{
		LOG_INFO( LoggingChannels::Server, "Client ({}) disconnected", index );
		const auto client_id = server.GetClientId( index );

		if (const auto entity_it = client_entities.find( index ); entity_it != std::end( client_entities ))
		{
			entity_it->second.destroy();
			client_entities.erase( entity_it );
		}

		client_id_index_mapping.erase( client_id );
	}

	void Server::TickSimulation( const PreciseTimestep& ts )
	{
		// TODO: call systems
		(void)ts;
	}
}
