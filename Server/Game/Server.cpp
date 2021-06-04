#include "Server.hpp"

#include "ClientServerCommon/Game/Networking/Config.hpp"
#include "Server/Game/Networking/Constants.hpp"

#include "Common/Utility/Containers.hpp"
#include "Common/Utility/MagicEnum.hpp"
#include "Common/Utility/StringUtility.hpp"

namespace Game
{
	Server::Server( Core& core )
		: core( core )
		, server( yojimbo::GetDefaultAllocator(), Networking::DefaultPrivateKey.data(), yojimbo::Address( "0.0.0.0" ), Networking::MakeConfiguration(), network_adapter, 0.0 )
		, network_adapter()
		, registry()
	{
		// Setup the networking server
		network_adapter.ServerClientConnected.connect( &Server::ClientConnectedHandler, this );
		network_adapter.ServerClientDisconnected.connect( &Server::ClientDisconnectedHandler, this );
		unauthed_clients.reserve( Networking::MaxServerClients );
		active_clients.reserve( Networking::MaxServerClients );
	}

	Server::~Server()
	{
		if (server.IsRunning())
			server.Stop();
	}

	Networking::ClientHandle Server::GetClientByIndex( Networking::ClientIndex client_index )
	{
		if (server.IsRunning() && server.IsClientConnected( client_index ))
			return Networking::ClientHandle( *this, server.GetClientId( client_index ) );

		return Networking::ClientHandle::Null;
	}

	Networking::ClientHandle Server::GetClientById( const Networking::ClientId& client_id )
	{
		if (const auto found_it = client_id_index_mapping.find( client_id ); found_it != std::end( client_id_index_mapping ))
			return GetClientByIndex( found_it->second );

		return Networking::ClientHandle::Null;
	}

	void Server::OnFixedUpdate( const PreciseTimestep& ts )
	{
		if (server.IsRunning())
		{
			server.ReceivePackets();
			ProcessMessages();
		}

		TickSimulation( ts );

		if (server.IsRunning())
			server.SendPackets();

		server.AdvanceTime( server.GetTime() + ts.delta );
	}

	void Server::OnVariableUpdate( const PreciseTimestep& )
	{
	}

	void Server::ClientConnectedHandler( Networking::Adapter&, Networking::ClientIndex index )
	{
		LOG_INFO( LoggingChannels::Server, "Client ({}) connected", index );
		const auto [it, success] = unauthed_clients.try_emplace( index, Networking::LoginClientState() );
		ASSERT( success );
		if (success)
		{
			client_id_index_mapping[server.GetClientId( index )] = index;
		}
		else
			server.DisconnectClient( index );
	}

	void Server::ClientDisconnectedHandler( Networking::Adapter&, Networking::ClientIndex index )
	{
		LOG_INFO( LoggingChannels::Server, "Client ({}) disconnected", index );
		unauthed_clients.erase( index );
		active_clients.erase( index );
		client_id_index_mapping.erase( index );
	}

	void Server::ProcessMessages()
	{
		ASSERT( server.IsRunning() );

		for (int idx = 0, max_clients = server.GetMaxClients(); idx < max_clients; idx++)
		{
			if (!server.IsClientConnected( idx ))
				continue;

			for (auto channel : magic_enum::enum_values<Networking::ChannelType>())
			{
				const auto channel_idx = static_cast<YojimboPlugin::ClientIndex_T>(channel);
				yojimbo::Message* message = server.ReceiveMessage( idx, channel_idx );
				while (message != NULL)
				{
					const auto message_type = message->GetType();
					const bool handled = HandleMessage( idx, *message );
					server.ReleaseMessage( idx, message );
					message = NULL;

					if (!handled)
					{
						LOG_CRITICAL( LoggingChannels::Server, "Disconnecting client ({}) due to unhandled message with type '{}'", idx, message_type );
						server.DisconnectClient( idx );
					}
				}
			}
		}
	}

	bool Server::HandleMessage( Networking::ClientIndex index, const yojimbo::Message& message )
	{
		if (!server.IsRunning() || server.IsClientConnected( index ))
			return false;


		if (const auto login_it = unauthed_clients.find( index ); login_it != std::end( unauthed_clients ))
		{
			// TODO
		}
		else if (const auto active_it = active_clients.find( index ); active_it != std::end( active_clients ))
		{
			// TODO
		}

		return false;
	}

	void Server::TickSimulation( const PreciseTimestep& ts )
	{
		// TODO: call systems
	}
}
