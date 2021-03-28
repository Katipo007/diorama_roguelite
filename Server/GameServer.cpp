#include "GameServer.hpp"

#include "ClientServerCommon/Game/Constants.hpp"
#include "ClientServerCommon/Networking/MessageFactory.hpp"

#include "Server/Sessions/ActiveClientSession.hpp"

namespace Server
{
	///
	/// ADAPTER
	/// 

	yojimbo::MessageFactory* GameServer::ConnectionAdapter::CreateMessageFactory( yojimbo::Allocator& allocator )
	{
		return YOJIMBO_NEW( allocator, Networking::ClientServerGameMessageFactory, allocator );
		// TODO: do I need a destructor for this??
	}

	void Server::GameServer::ConnectionAdapter::OnServerClientConnected( int client_index )
	{
		server.OnClientConnected( client_index );
	}

	void GameServer::ConnectionAdapter::OnServerClientDisconnected( int client_index )
	{
		server.OnClientDisconnected( client_index );
	}


	/// 
	/// SERVER
	/// 

	GameServer::GameServer( const yojimbo::Address& server_address, const uint16_t max_connected_clients )
		: adapter( *this )
		, connection_config()
		, server( yojimbo::GetDefaultAllocator(), Networking::ClientServerGameConnectionConfig::DefaultPrivateKey, server_address,  connection_config, adapter, 0.0 )
		, connected_clients()
		, max_connected_clients( max_connected_clients )
	{
		ASSERT( max_connected_clients > 0 );
		LOG_INFO( Server, "Server initialising" );

		connected_clients.reserve( max_connected_clients );
		server.Start( (int)max_connected_clients );

		char server_address_buffer[256];
		server.GetAddress().ToString( server_address_buffer, sizeof( server_address_buffer ) );

		if (!server.IsRunning())
			throw std::runtime_error( "Could not start server on provided address. '" + std::string(server_address_buffer) + "'" );

		LOG_INFO( Server, "Server started on '{}' (insecure)", server_address_buffer );

		// TODO: initialise and run game world
	}

	GameServer::~GameServer()
	{
		if (server.IsRunning())
			server.Stop();
		
		ASSERT( !server.IsRunning() );
		LOG_INFO( Server, "Server stopped" );
	}

	bool GameServer::IsRunning() const
	{
		return server.IsRunning();
	}

	void GameServer::OnClientConnected( ClientIndex_T client_index )
	{
		try
		{
			const auto id = server.GetClientId( client_index );
			auto session = std::make_unique<Sessions::ActiveClientSession>( *this, client_index, id );
			const auto [it, successful] = connected_clients.try_emplace( id, std::move( session ) );

			if (!successful || session != nullptr)
				throw std::runtime_error( "Error adding new active client session" );

			// TODO: notify other clients of new connection
			// TODO: other stuff

			LOG_INFO( Server, "Client connected. Index: '{}', Id: '{}'", client_index, id );
		}
		catch (std::runtime_error& e)
		{
			LOG_ERROR( Server, "Exception initialising client connection, severing connection. What: {}", e.what() );
			server.DisconnectClient( client_index );
		}
	}

	void GameServer::OnClientDisconnected( ClientIndex_T client_index )
	{
		// Remove active session object
		const auto client_id = server.GetClientId( client_index );

		auto client_it = connected_clients.find( client_id );
		if (client_it != std::end( connected_clients ))
		{
			connected_clients.erase( client_it );
			LOG_INFO( Server, "Client disconnected. Index: '{}', Id: '{}'", client_index, client_id );
		}
	}

	void GameServer::Update( const PreciseTimestep& timestep )
	{
		if (!server.IsRunning())
			return;
		
		// Receive and process messages
		server.ReceivePackets();
		ProcessMessages();

		// TODO: process client inputs
		
		// TODO: update game
		
		// Update clients
		for (auto& it : connected_clients)
		{
			auto& session = *(it.second);
			session.Update( timestep );
		}

		// TODO: send game updates to clients

		// send packets
		server.SendPackets();

		// gracefully disconnect marked clients
		{
			auto it = std::begin( connected_clients );
			while (it != std::end( connected_clients ))
			{
				const auto& session = *(it->second);
				// should be disconnected
				if (session.IsMarkedForDisconnect())
				{
					++it;
					server.DisconnectClient( session.GetClientIndex() );
				}
				else
					++it;
			}
		}

		// tick server
		server.AdvanceTime( server.GetTime() + timestep.delta );
	}

	ClientId_T GameServer::GetClientIdByIndex( const ClientIndex_T& client_index ) const
	{
		if (server.IsClientConnected( client_index ))
			return server.GetClientId( client_index );

		return InvalidClientIndex;
	}

	ClientIndex_T GameServer::GetClientIndexById( const ClientId_T& client_id ) const
	{
		const auto it = std::find_if( std::begin( connected_clients ), std::end( connected_clients ), [&client_id]( const ClientContainer_T::value_type& entry )
			{
				return entry.second->GetClientId() == client_id;
			} );

		if (it != std::end( connected_clients ))
			return it->second->GetClientIndex();

		return InvalidClientIndex;
	}

	Sessions::ActiveClientSession* GameServer::GetClientById( const ClientId_T& client_id )
	{
		if (auto it = connected_clients.find( client_id ); it != std::end( connected_clients ))
			return it->second.get();

		return nullptr;
	}

	const Sessions::ActiveClientSession* GameServer::GetClientById( const ClientId_T& client_id ) const
	{
		if (auto it = connected_clients.find( client_id ); it != std::end( connected_clients ))
			return it->second.get();

		return nullptr;
	}

	Sessions::ActiveClientSession* GameServer::GetClientByIndex( const ClientIndex_T& client_id )
	{
		auto it = std::find_if( std::begin( connected_clients ), std::end( connected_clients ), [&client_id]( const ClientContainer_T::value_type& entry )
			{
				return entry.second->GetClientId() == client_id;
			} );

		if (it != std::end( connected_clients ))
			return it->second.get();

		return nullptr;
	}

	const Sessions::ActiveClientSession* GameServer::GetClientByIndex( const ClientIndex_T& client_id ) const
	{
		auto it = std::find_if( std::begin( connected_clients ), std::end( connected_clients ), [&client_id]( const ClientContainer_T::value_type& entry )
			{
				return entry.second->GetClientId() == client_id;
			} );

		if (it != std::end( connected_clients ))
			return it->second.get();

		return nullptr;
	}

	void GameServer::ForEachClient( const std::function<void( Sessions::ActiveClientSession& )>& func )
	{
		std::for_each( std::begin( connected_clients ), std::end( connected_clients ), [&func]( ClientContainer_T::value_type& entry )
			{
				func( *entry.second );
			} );
	}

	void GameServer::ForEachClient( const std::function<void( const Sessions::ActiveClientSession& )>& func ) const
	{
		std::for_each( std::begin( connected_clients ), std::end( connected_clients ), [&func]( const ClientContainer_T::value_type& entry )
			{
				func( *entry.second );
			} );
	}

	void GameServer::DisconnectClient( Sessions::ActiveClientSession& session, std::optional<std::string> reason )
	{
		ASSERT( OwnsClient( session ) );

		session.SessionFlags |= Sessions::ActiveClientSession::Flags::MarkedForDisconnection;

		if (reason)
		{
			// TODO: store reason text
			// TODO: send reason message to client
		}
	}

	void GameServer::SeverClient( Sessions::ActiveClientSession& session )
	{
		ASSERT( OwnsClient( session ) );

		server.DisconnectClient( session.GetClientIndex() );
	}

	bool GameServer::SendMessage(
		const Sessions::ActiveClientSession& session
		, const Networking::MessageId::MessageId message_id
		, const Networking::ChannelType::Type channel
		, const std::function<void( yojimbo::Message&, const Sessions::ActiveClientSession& )>& message_init_func
	)
	{
		ASSERT( OwnsClient( session ) );

		if (!server.IsClientConnected( session.GetClientIndex() ) || session.IsMarkedForDisconnect())
			return false;

		auto* message = server.CreateMessage( session.GetClientIndex(), message_id );
		ASSERT( message != nullptr );
		if (message == nullptr)
			return false;

		message_init_func( *message, session );
		server.SendMessage( session.GetClientIndex(), channel, message );

		return true;
	}

	size_t GameServer::BroadcastMessage(
		const Networking::MessageId::MessageId message_id
		, const Networking::ChannelType::Type channel
		, const std::function<void( yojimbo::Message&, const Sessions::ActiveClientSession& )>& message_init_func
		, const std::function<bool( const Sessions::ActiveClientSession& )> inclusion_predicate
	)
	{
		size_t count = 0;
		ForEachClient( [ & ]( const Sessions::ActiveClientSession& session )
			{
				if (inclusion_predicate( session ))
				{
					if (SendMessage( session, message_id, channel, message_init_func ))
						++count;
				}
			} );

		return count;
	}

	void GameServer::ProcessMessages()
	{
		// TODO: Profile

		yojimbo::Message* message = nullptr;

		// for every client
		auto client_it = std::begin( connected_clients );
		while (client_it != std::end( connected_clients ))
		{
			auto& session = *client_it->second;
			const auto client_index = session.GetClientIndex();
			const auto client_id = session.GetClientId();
			++client_it; // iterate now so if the current session disappears we can still continue

			// if connected
			if (server.IsClientConnected( client_index ) && !session.IsMarkedForDisconnect())
			{
				// process messages from all channels
				for (int channel = 0; channel < connection_config.numChannels; ++channel)
				{
					message = server.ReceiveMessage( client_index, channel );
					while (message != nullptr)
					{
						session.ProcessMessage( *message );
						server.ReleaseMessage( client_index, message );
						message = server.ReceiveMessage( client_index, channel );
					}
				}
			}
		}
	}

	bool GameServer::OwnsClient( const Sessions::ActiveClientSession& session ) const
	{
		const auto it = std::find_if( std::begin( connected_clients ), std::end( connected_clients ), [&session]( const ClientContainer_T::value_type& entry )
			{
				return entry.second.get() == &session;
			}
		);

		if (it != std::end( connected_clients ))
			return true;

		return false;
	}
}
