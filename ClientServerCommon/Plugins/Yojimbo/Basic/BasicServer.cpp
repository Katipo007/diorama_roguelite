#include "BasicServer.hpp"

#include <limits>

#include "../MessageFactory.hpp"
#include "../Message.hpp"

namespace YojimboPlugin
{
	BasicServer::ClientEntry::ClientEntry( int yojimbo_index_, ClientId_T id_, ClientConnection&& connection_ )
		: connection( std::move( connection_ ) )
		, yojimbo_index( yojimbo_index_ )
		, id( id_ )
	{
	}


	BasicServer::BasicServer(
		yojimbo::Address&& host_address_
		, size_t max_connected_clients_
		, const std::array<uint8_t, yojimbo::KeyBytes>& private_key_
		, yojimbo::ClientServerConfig&& config_
		, BasicAdapter&& adapter_
	)
		: config( std::move( config_ ) )
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
			adapter.ServerClientConnected.connect( [this]( BasicAdapter&, int index )
				{
					const auto client_id = static_cast<ClientId_T>(server.GetClientId( index ));
					LOG_TRACE( Server, "New client connection, Index: '{}', Id: '{}'", index, client_id );

					try
					{
						auto new_connection = ClientConnection( *this, client_id );// , client_id, index );
						auto [it, success] = clients.try_emplace( client_id, ClientEntry( index, client_id, std::move( new_connection ) ) );
						if (!success)
							throw std::runtime_error( "Failed to emplace new client connection in container" );

						ClientConnected( *this, it->second.connection );
					}
					catch (std::runtime_error& e)
					{
						LOG_ERROR( Server, "Exception initialising new client connection: {}", e.what() );
						server.DisconnectClient( index );
					}
				} );

			adapter.ServerClientDisconnected.connect( [this]( BasicAdapter&, int index )
				{
					// may not exist if there was a probleem during ClientConnectionEstablished
					auto it = std::find_if( std::begin( clients ), std::end( clients ), [&index]( const ClientContainer_T::value_type& entry )
						{
							return entry.second.yojimbo_index == index;
						} );
					if (it != std::end( clients ))
					{
						ClientDisconnected( *this, it->second.connection );
						clients.erase( it );
					}

					LOG_TRACE( Server, "Client index '{}' disconnected", index );
				} );
		}

		
		LOG_INFO( Server, "Server started on '{}' (insecure)", server_address_buffer );
	}

	BasicServer::~BasicServer()
	{
		if (server.IsRunning())
			server.Stop();

		ASSERT( !server.IsRunning() );
		LOG_INFO( Server, "Server stopped" );
	}

	bool BasicServer::IsRunning() const noexcept
	{
		return server.IsRunning();
	}

	size_t BasicServer::GetMaxConnectedClients() const noexcept
	{
		return static_cast<size_t>(server.GetMaxClients());
	}

	size_t BasicServer::GetNumConnectedClients() const noexcept
	{
		return static_cast<size_t>(server.GetNumConnectedClients());
	}

	bool BasicServer::IsDisconnecting( const ClientConnection& client ) const
	{
		if (auto* entry = FindClientEntry( client ))
			return entry->is_disconnecting;

		return false;
	}

	void BasicServer::DisconnectClient( ClientConnection& client, std::optional<std::string> reason, bool sever )
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

	bool BasicServer::SendMessage( MessageType type, ChannelType channel, ClientConnection& client, const MessageInitialiserFunc_T& initialiser )
	{
		ASSERT( (bool)initialiser );
		ASSERT( server.IsRunning() );
		if (!server.IsRunning())
			return false;

		auto* entry = FindClientEntry( client );
		if (!entry)
		{
			ASSERT( false, "Trying to send message to unowned connection" );
			return false; // silently fail
		}

		if (entry->is_disconnecting)
			return false; // don't send messages to clients which are disconnecting

		//
		// COMMITTED PAST HERE
		//
		ASSERT( server.IsClientConnected( entry->yojimbo_index ) );

		auto* const yojimbo_message = server.CreateMessage( entry->yojimbo_index, type );
		auto* const plugin_message = dynamic_cast<Message*>(yojimbo_message);
		if (!yojimbo_message || !plugin_message)
			throw std::runtime_error( "Failed to allocate message" );

		initialiser( client, *plugin_message );
		server.SendMessage( entry->yojimbo_index, static_cast<int>(channel), yojimbo_message );

		return true;
	}

	size_t BasicServer::BroadcastMessage( MessageType type, ChannelType channel, const MessageInitialiserFunc_T& initialiser, const ClientConnectionPredicate_T& predicate )
	{
		ASSERT( initialiser );
		ASSERT( predicate );
		ASSERT( server.IsRunning() );
		if (!server.IsRunning())
			return 0;

		size_t n = 0;
		for (auto& [id, entry] : clients)
		{
			if (entry.is_disconnecting || !predicate( entry.connection ))
				continue;

			if (SendMessage( type, channel, entry.connection, initialiser ))
				++n;
		}

		return n;
	}

	void BasicServer::Update( const PreciseTimestep& timestep )
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
		ASSERT( timestep.delta > 0 );
		server.AdvanceTime( timestep.delta );
	}

	void BasicServer::ProcessMessages()
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
						try
						{
							if( !HandleMessage( entry.connection, dynamic_cast<Message&>(*yojimbo_message) ) )
								DisconnectClient( entry.connection, "Unhandled message", true ); // sever
						}
						catch (std::bad_cast& e)
						{
							LOG_CRITICAL( Server, "Failed to parse message: '{}'", e.what() );
							DisconnectClient( entry.connection, "Bad message", true );
						}

						server.ReleaseMessage( index, yojimbo_message );
						yojimbo_message = server.ReceiveMessage( index, channel );
					}
				}
			}
		}
	}

	bool BasicServer::OwnsConnection( const ClientConnection& client ) const noexcept
	{
		return FindClientEntry( client ) != nullptr;
	}

	ClientId_T BasicServer::GetClientId( const ClientConnection& client ) const
	{
		if( auto* entry = FindClientEntry( client ) )
			return entry->id;

		throw std::runtime_error( "Server does not own this connection" );
	}

	ClientConnection* BasicServer::GetClientById( ClientId_T id ) noexcept
	{
		if (auto* entry = FindClientEntryById( id ))
			return &entry->connection;
		return nullptr;
	}

	const ClientConnection* BasicServer::GetClientById( ClientId_T id ) const noexcept
	{
		if (auto* entry = FindClientEntryById( id ))
			return &entry->connection;
		return nullptr;
	}

	const ClientConnection* BasicServer::FindClient( const ClientConnectionPredicate_T& predicate ) const
	{
		auto it = std::find_if( std::begin( clients ), std::end( clients ), [&]( const ClientContainer_T::value_type& entry )
			{
				return !entry.second.is_disconnecting && predicate( entry.second.connection );
			} );
		if (it != std::end( clients ))
			return &(it->second.connection);

		return nullptr;
	}

	void BasicServer::ForEachClient( const std::function<void( ClientConnection& connection )>& func )
	{
		for (auto& it : clients)
		{
			auto& entry = it.second;
			if (!entry.is_disconnecting)
				func( entry.connection );
		}
	}

	void BasicServer::ForEachClient( const std::function<void( const ClientConnection& connection )>& func ) const
	{
		for (auto& it : clients)
		{
			auto& entry = it.second;
			if (!entry.is_disconnecting)
				func( entry.connection );
		}
	}

	const BasicServer::ClientEntry* BasicServer::FindClientEntryById( ClientId_T client_id ) const
	{
		auto it = clients.find( client_id );
		if (it != std::end( clients ))
			return &(it->second);

		return nullptr;
	}

	const BasicServer::ClientEntry* BasicServer::FindClientEntryByIndex( int client_index ) const
	{
		return FindClientEntryById( server.GetClientId( client_index) );
	}

	const BasicServer::ClientEntry* BasicServer::FindClientEntry( const ClientConnection& client ) const
	{
		auto entry_it = clients.find( client.GetId() );

		if ((entry_it != std::end( clients )))
		{
			auto& entry = entry_it->second;
			if (&entry.connection == &client)
				return &entry;
		}

		return nullptr;
	}
}
