#include "ServerYojimbo.hpp"

#include <limits>

#include "Common/Networking/MessageFactory.hpp"

#include "YojimboNetworkAdapters.hpp"

namespace Plugins::Network::Yojimbo
{
	ServerYojimbo::ClientEntry::ClientEntry( int yojimbo_index_, ::Networking::ClientId_T id_, ClientConnectionYojimbo&& connection_ )
		: connection( std::move( connection_ ) )
		, yojimbo_index( yojimbo_index_ )
		, id( id_ )
	{
	}


	ServerYojimbo::ServerYojimbo(
		yojimbo::Address&& host_address_
		, size_t max_connected_clients_
		, const std::array<uint8_t, yojimbo::KeyBytes>& private_key_
		, yojimbo::ClientServerConfig&& config_
		, ServerAdapter&& adapter_
	)
		: ::Networking::Server()
		, config( std::move( config_ ) )
		, adapter( std::move( adapter_ ) )
		, server( yojimbo::GetDefaultAllocator(), private_key_.data(), host_address_, config, adapter, 0.0 )
	{
		///
		/// Setup memory
		/// 
		ASSERT( max_connected_clients_ > 0 );
		ASSERT( max_connected_clients_ < std::numeric_limits<int>::max() );
		clients.reserve( max_connected_clients_ );

		///
		/// Start the yojimbo server
		/// 
		server.Start( static_cast<int>(max_connected_clients_) );
		char server_address_buffer[256] = "\0";

		// check the yojimbo server started
		{
			server.GetAddress().ToString( server_address_buffer, sizeof( server_address_buffer ) );

			if (!server.IsRunning())
				throw std::runtime_error( "Could not start server on '" + std::string( server_address_buffer ) + "'" );
		}

		///
		/// Hookup event handlers
		///
		{
			adapter.ClientConnectionEstablished.connect( [this]( ServerAdapter&, int index )
				{
					const auto client_id = static_cast<::Networking::ClientId_T>(server.GetClientId( index ));
					LOG_TRACE( Server, "New client connection, Index: '{}', Id: '{}'", index, client_id );

					try
					{
						auto new_connection = ClientConnectionYojimbo( *this, client_id, index );
						auto [it, success] = clients.try_emplace( client_id, ClientEntry( index, client_id, std::move( new_connection ) ) );
						if (!success)
							throw std::runtime_error( "Failed to emplace new client connection in container" );

						ClientIntroduced( *this, it->second.connection );
					}
					catch (std::runtime_error& e)
					{
						LOG_ERROR( Server, "Exception initialising new client connection: {}", e.what() );
						server.DisconnectClient( index );
					}
				} );

			adapter.ClientConnectionLost.connect( [this]( ServerAdapter&, int index )
				{
					// may not exist if there was a probleem during ClientConnectionEstablished
					auto it = std::find_if( std::begin( clients ), std::end( clients ), [&index]( const ClientContainer_T::value_type& entry )
						{
							return entry.second.yojimbo_index == index;
						} );
					if (it != std::end( clients ))
					{
						ClientDisconnected( *this, it->second.connection, it->second.state );
						clients.erase( it );
					}

					LOG_TRACE( Server, "Client index '{}' disconnected", index );
				} );
		}

		
		LOG_INFO( Server, "Server started on '{}' (insecure)", server_address_buffer );
	}

	ServerYojimbo::~ServerYojimbo()
	{
		if (server.IsRunning())
			server.Stop();

		ASSERT( !server.IsRunning() );
		LOG_INFO( Server, "Server stopped" );
	}

	bool ServerYojimbo::IsRunning() const noexcept
	{
		return server.IsRunning();
	}

	size_t ServerYojimbo::GetMaxConnectedClients() const noexcept
	{
		return static_cast<size_t>(server.GetMaxClients());
	}

	size_t ServerYojimbo::GetNumConnectedClients() const noexcept
	{
		return static_cast<size_t>(server.GetNumConnectedClients());
	}

	::Networking::ClientConnectionState ServerYojimbo::GetClientState( const::Networking::ClientConnection& client ) const
	{
		ASSERT( server.IsRunning() );
		if (auto* entry = FindClientEntry( client ))
			return entry->state;

		return ::Networking::ClientConnectionState::Disconnected;
	}

	void ServerYojimbo::IncrementClientState( ::Networking::ClientConnection& client, ::Networking::ClientConnectionState new_state )
	{
		ASSERT( server.IsRunning() );
		auto entry = FindClientEntry( client );
		if (!entry)
			throw std::runtime_error( "Server does not own this connection" );

		if (new_state <= entry->state)
			throw std::runtime_error( "IncrementClientState cannot be used to decrement the client state" );

		entry->state = new_state;

		switch (entry->state)
		{
		case ::Networking::ClientConnectionState::Authenticated:
			ClientAuthenticated( *this, entry->connection );
			break;

		case ::Networking::ClientConnectionState::Accepted:
			ClientAccepted( *this, entry->connection );
			break;

		case ::Networking::ClientConnectionState::Disconnected:
			// unconventional way of disconnecting the client but we'll allow it
			DisconnectClient( entry->connection, "" );
			break;

		default:
		case ::Networking::ClientConnectionState::Default:
			throw std::runtime_error( "Invalid state" );
			break;
		}
	}

	void ServerYojimbo::DisconnectClient( ::Networking::ClientConnection& client, std::optional<std::string_view> reason, bool sever )
	{
		ASSERT( server.IsRunning() );
		auto* entry = FindClientEntry( client );
		if (!entry)
			throw std::runtime_error( "Server does not own this connection" );

		if (sever)
		{
			entry->is_disconnecting = true;
			server.DisconnectClient( entry->yojimbo_index );
		}
		else
		{
			entry->is_disconnecting = true;

			(void)reason;
			// TODO: send "you are getting disconnected" message with the reason
			NOT_IMPLEMENTED;
		}
	}

	bool ServerYojimbo::SendMessage( std::unique_ptr<::Networking::Message> message, ::Networking::ChannelType channel, ::Networking::ClientConnection& client )
	{
		ASSERT( server.IsRunning() );
		ASSERT( message );
		if (!message || !server.IsRunning())
			return false;

		auto* entry = FindClientEntry( client );
		if (!entry)
		{
			ASSERT( false, "Trying to send message to unowned connection" );
			return false; // silently fail
		}

		if (entry->is_disconnecting)
			return false; // don't send messages to clients which are getting disconnected

		//
		// COMMITTED PAST HERE
		//
		ASSERT( server.IsClientConnected( entry->yojimbo_index ) );

		int yojimbo_message_type = -1; // TODO:
		auto* yojimbo_message = server.CreateMessage( entry->yojimbo_index, yojimbo_message_type );
		ASSERT( yojimbo_message );
		if (!yojimbo_message)
		{
			LOG_ERROR( Server, "Failed to allocate yojimbo message" );
			return false;
		}

		server.SendMessage( entry->yojimbo_index, static_cast<int>(channel), yojimbo_message );

		return true;
	}

	size_t ServerYojimbo::BroadcastMessage( std::unique_ptr<::Networking::Message> message, ::Networking::ChannelType channel, const std::function<bool( const::Networking::ClientConnection& )>& predicate )
	{
		ASSERT( server.IsRunning() );
		ASSERT( message );
		if (!message || !server.IsRunning())
			return 0;

		size_t n = 0;

		for (auto& [id, entry] : clients)
		{
			if (entry.is_disconnecting || (entry.state != ::Networking::ClientConnectionState::Accepted) || !predicate( entry.connection ))
				continue;

			if (SendMessage( std::move( message ), channel, entry.connection )) // TODO: make this not a unique_ptr for obvious reasons
				++n;
		}

		return n;
	}

	void ServerYojimbo::Update( const PreciseTimestep& timestep )
	{
		if (server.IsRunning())
		{
			// receive and process messages
			server.ReceivePackets();
			ProcessMessages();

			// update clients
			for (auto& [id, entry] : clients)
			{
				// TODO: disconnect clients which have taken too long to authenticate or get accepted
			}

			// send packets
			server.SendPackets();

			// gracefully disconnect marked clients
			{
				auto it = std::begin( clients );
				while (it != std::end( clients ))
				{
					auto& [id, entry] = *it;
					if (entry.is_disconnecting)
					{
						++it;
						server.DisconnectClient( entry.yojimbo_index );
					}
					else
						++it;
				}
			}
		}

		// tick server
		ASSERT( timestep.delta >= 0.0 );
		server.AdvanceTime( timestep.delta );
	}

	void ServerYojimbo::ProcessMessages()
	{
		ASSERT( server.IsRunning() );

		yojimbo::Message* yojimbo_message = nullptr;
		
		auto it = std::begin( clients );
		while (it != std::end( clients ))
		{
			auto& [id, entry] = *it;
			++it; // iterate now so if the current client disappears we can still iterate safely

			const auto index = entry.yojimbo_index;
			if (server.IsClientConnected( index ) && !entry.is_disconnecting)
			{
				for (int channel = 0; channel < config.numChannels; channel++)
				{
					yojimbo_message = server.ReceiveMessage( index, channel );
					while (yojimbo_message != nullptr && server.IsClientConnected( index ) && !entry.is_disconnecting)
					{
						::Networking::Message message; // TODO:
						NOT_IMPLEMENTED;

						const bool handled = entry.connection.ProcessMessage( message );
						if (!handled)
							DisconnectClient( entry.connection, "Unhandled message", true ); // sever

						server.ReleaseMessage( index, yojimbo_message );
						yojimbo_message = server.ReceiveMessage( index, channel );
					}
				}
			}
		}
	}

	bool ServerYojimbo::OwnsConnection( const::Networking::ClientConnection& client ) const noexcept
	{
		return FindClientEntry( client ) != nullptr;
	}

	::Networking::ClientId_T ServerYojimbo::GetClientId( const::Networking::ClientConnection& client ) const
	{
		if( auto* entry = FindClientEntry( client ) )
			return entry->id;

		throw std::runtime_error( "Server does not own this connection" );
	}

	::Networking::ClientConnection* ServerYojimbo::GetClientById( ::Networking::ClientId_T id ) noexcept
	{
		if (auto* entry = FindClientEntryById( id ))
			return &entry->connection;
		return nullptr;
	}

	const::Networking::ClientConnection* ServerYojimbo::GetClientById( ::Networking::ClientId_T id ) const noexcept
	{
		if (auto* entry = FindClientEntryById( id ))
			return &entry->connection;
		return nullptr;
	}

	::Networking::ClientConnection* ServerYojimbo::FindClient( ::Networking::ClientConnectionState state, const std::function<bool( const::Networking::ClientConnection& )>& predicate )
	{
		auto it = std::find_if( std::begin( clients ), std::end( clients ), [&]( const ClientContainer_T::value_type& entry )
			{
				return (entry.second.state == state) && !entry.second.is_disconnecting && predicate( entry.second.connection );
			} );
		if (it != std::end( clients ))
			return &(it->second.connection);

		return nullptr;
	}

	const::Networking::ClientConnection* ServerYojimbo::FindClient( ::Networking::ClientConnectionState state, const std::function<bool( const::Networking::ClientConnection& )>& predicate ) const
	{
		auto it = std::find_if( std::begin( clients ), std::end( clients ), [&]( const ClientContainer_T::value_type& entry )
			{
				return (entry.second.state == state) && !entry.second.is_disconnecting && predicate( entry.second.connection );
			} );
		if (it != std::end( clients ))
			return &(it->second.connection);

		return nullptr;
	}

	void ServerYojimbo::ForEachClient( ::Networking::ClientConnectionState state, const std::function<void( ::Networking::ClientConnection& connection )>& func )
	{
		for (auto& it : clients)
		{
			auto& entry = it.second;
			if ((entry.state == state) && !entry.is_disconnecting)
				func( entry.connection );
		}
	}

	void ServerYojimbo::ForEachClient( ::Networking::ClientConnectionState state, const std::function<void( const::Networking::ClientConnection& connection )>& func ) const
	{
		for (auto& it : clients)
		{
			auto& entry = it.second;
			if ((entry.state == state) && !entry.is_disconnecting)
				func( entry.connection );
		}
	}

	void ServerYojimbo::ForEachClient( const std::function<void( ::Networking::ClientConnection& connection, ::Networking::ClientConnectionState state )>& func )
	{
		for (auto& it : clients)
		{
			auto& entry = it.second;
			if (!entry.is_disconnecting)
				func( entry.connection, entry.state );
		}
	}

	void ServerYojimbo::ForEachClient( const std::function<void( const::Networking::ClientConnection& connection, ::Networking::ClientConnectionState state )>& func ) const
	{
		for (auto& it : clients)
		{
			auto& entry = it.second;
			if (!entry.is_disconnecting)
				func( entry.connection, entry.state );
		}
	}

	ServerYojimbo::ClientEntry* ServerYojimbo::FindClientEntryById( ::Networking::ClientId_T client_id )
	{
		auto it = clients.find( client_id );
		if (it != std::end( clients ))
			return &(it->second);

		return nullptr;
	}

	const ServerYojimbo::ClientEntry* ServerYojimbo::FindClientEntryById( ::Networking::ClientId_T client_id ) const
	{
		auto it = clients.find( client_id );
		if (it != std::end( clients ))
			return &(it->second);

		return nullptr;
	}

	ServerYojimbo::ClientEntry* ServerYojimbo::FindClientEntryByIndex( int client_index )
	{
		return FindClientEntryById( server.GetClientId( client_index) );
	}

	const ServerYojimbo::ClientEntry* ServerYojimbo::FindClientEntryByIndex( int client_index ) const
	{
		return FindClientEntryById( server.GetClientId( client_index) );
	}

	ServerYojimbo::ClientEntry* ServerYojimbo::FindClientEntry( const ::Networking::ClientConnection& client )
	{
		ASSERT( OwnsConnection( client ) );
		return FindClientEntryById( client.GetId() );
	}

	const ServerYojimbo::ClientEntry* ServerYojimbo::FindClientEntry( const::Networking::ClientConnection& client ) const
	{
		ASSERT( OwnsConnection( client ) );
		return FindClientEntryById( client.GetId() );
	}
}
