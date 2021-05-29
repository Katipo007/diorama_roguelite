#include "GameServer.hpp"

#include "ClientServerCommon/Networking/ClientServer/Config.hpp"
#include "ClientServerCommon/Networking/ClientServer/MessageFactory.hpp"
#include "Server/Networking/ClientServer/ActiveClient.hpp"
#include "Server/Networking/ClientServer/UnauthenticatedClient.hpp"

#include "Common/Utility/Containers.hpp"
#include "Common/Utility/MagicEnum.hpp"

using namespace Networking::ClientServer;

bool GameServer::ClientEntry::IsActive() const noexcept
{
	return !to_be_disconnected && (GetActive() != nullptr);
}

bool GameServer::ClientEntry::IsValid() const noexcept
{
	return (GetActive() != nullptr) || (GetUnauthed() != nullptr);
}

Networking::ClientServer::ActiveClient* GameServer::ClientEntry::GetActive() const noexcept
{
	if (auto* active_client = std::get_if<ActiveClientPtr>( &client ))
		return active_client->get();

	return nullptr;
}

Networking::ClientServer::UnauthenticatedClient* GameServer::ClientEntry::GetUnauthed() const noexcept
{
	if (auto* unauthed_client = std::get_if<UnauthenticatedClientPtr>( &client ))
		return unauthed_client->get();

	return nullptr;
}

GameServer::ClientEntry::ClientEntry( const time_t connected_timestamp, std::variant< UnauthenticatedClientPtr, ActiveClientPtr > client )
	: connected_timestamp( connected_timestamp)
	, client( std::move( client ) )
{
	ASSERT( IsValid() );
}



//
// GameServer
//

GameServer::GameServer( yojimbo::Address host_address, const int max_clients )
	: server( yojimbo::GetDefaultAllocator(), Networking::ClientServer::DefaultPrivateKey.data(), host_address, Networking::ClientServer::MakeConfiguration(), adapter, 0.0 )
	, adapter()
{
	// Reserve memory
	clients.reserve( max_clients );

	// Connect listeners
	adapter.ServerClientConnected.connect( &GameServer::ClientConnectedHandler, this );
	adapter.ServerClientDisconnected.connect( &GameServer::ClientDisconnectedHandler, this );

	// Start the server
	server.Start( max_clients );
	std::array<char, 256> server_address_buffer{ "\0" };
	server.GetAddress().ToString( &server_address_buffer[0], static_cast<int>(std::size( server_address_buffer )) );

	if (!server.IsRunning())
		throw std::runtime_error( "Could not start server on '" + std::string( server_address_buffer.data() ) + "'" );
}

GameServer::~GameServer()
{
	if (server.IsRunning())
		server.Stop();

	ASSERT( !IsRunning() );
}

bool GameServer::IsRunning() const noexcept
{
	return server.IsRunning();
}

size_t GameServer::GetConnectedClientCount() const noexcept
{
	return server.GetNumConnectedClients();
}

size_t GameServer::GetMaxClientCount() const noexcept
{
	return server.GetMaxClients();
}

void GameServer::AcceptClient( Networking::ClientServer::UnauthenticatedClient& client )
{
	if (auto [idx, entry] = FindClientEntry( client ); !!entry)
	{
		if (auto* unauthed = entry->GetUnauthed())
			entry->client = std::make_unique<ActiveClient>( *unauthed );
		else
			LOG_CRITICAL( Server, "Client '{}'({}) isn't in the unauthenticated state", server.GetClientId( idx ), idx );
	}
}

void GameServer::RejectClient( Networking::ClientServer::UnauthenticatedClient& client )
{
	if (auto [idx, entry] = FindClientEntry( client ); !!entry)
	{
		if (auto* unauthed = entry->GetUnauthed())
			DisconnectClient( *entry, "Rejected" );
		else
			LOG_CRITICAL( Server, "Client '{}'({}) isn't in the unauthenticated state", server.GetClientId( idx ), idx );
	}
}

void GameServer::DisconnectClient( Networking::ClientServer::ActiveClient& client, std::optional<std::string> reason )
{
	ASSERT( &client.GetOwner() == this );
	
	const auto Pred = [&client]( const ClientContainer_T::value_type& value ) -> bool
	{
		if (auto* active_client = std::get_if<ActiveClientPtr>( &value.second.client ))
			return (active_client != nullptr) && (active_client->get() == &client);

		return false;
	};

	if (const auto found_it = std::find_if( std::begin( clients ), std::end( clients ), Pred ); found_it != std::end( clients ))
		DisconnectClient( found_it->second, reason );
}

void GameServer::DisconnectAllClients( std::optional<std::string> reason )
{
	for (auto& [client_index, client_entry] : clients)
		DisconnectClient( client_entry, reason );
}

void GameServer::OnFixedUpdate( const PreciseTimestep& ts )
{
	if (!IsRunning())
	{
		// TODO: should this throw? Do we want the server to be a state where it isn't running??
	}
	else
	{
		server.ReceivePackets();
		ProcessMessages();
		for (auto& [index, entry] : clients)
		{
			if (!entry.to_be_disconnected && entry.IsValid())
			{
				if (auto* unauthed = entry.GetUnauthed())
					unauthed->OnFixedUpdate( ts );
				else if (auto* active = entry.GetActive())
					active->OnFixedUpdate( ts );
			}
		}
		server.SendPackets();

		// Disconnect clients
		{
			auto client_it = std::begin( clients );
			while (client_it != std::end( clients ))
			{
				auto& [client_index, entry] = *client_it;
				if (!server.IsClientConnected( client_index ) || entry.to_be_disconnected || !entry.IsValid())
				{
					if (server.IsClientConnected( client_index ))
						server.DisconnectClient( client_index );
					client_it = clients.erase( client_it );
				}
				else
					++client_it;
			}
		}
	}

	server.AdvanceTime( server.GetTime() + ts.delta );
}

void GameServer::ForEachClient( std::invocable<Networking::ClientServer::ActiveClient&> auto func, std::predicate<const Networking::ClientServer::ActiveClient&> auto predicate )
{
	for (auto& [client_index, entry] : clients)
	{
		auto* const active = entry.GetActive();
		if (entry.IsValid() && !!active && predicate( *active ))
			func( *active );
	};
}

void GameServer::ClientConnectedHandler( Networking::ClientServer::ClientServerAdapter&, YojimboPlugin::ClientIndex_T index )
{
	const auto client_id = server.GetClientId( index );
	if (clients.contains( index ))
	{
		LOG_WARN( Server, "Already have a connected client with id '{}', disconnecting new client ({})", client_id, index );
		server.DisconnectClient( index );
		return;
	}

	const auto now = time( NULL );

	const auto [it, success] = clients.try_emplace( index, ClientEntry( now, std::make_unique<Networking::ClientServer::UnauthenticatedClient>( *this, index ) ) );
	if (!success)
	{
		LOG_WARN( Server, "Failed to allocate connection for client '{}'({})", client_id, index );
		server.DisconnectClient( index );
		return;
	}
	LOG_INFO( Server, "Client '{}'({}) connected", client_id, index );
	ASSERT( it->second.IsValid() );
}

void GameServer::ClientDisconnectedHandler( Networking::ClientServer::ClientServerAdapter&, YojimboPlugin::ClientIndex_T index )
{
	const auto found_it = clients.find( index );
	if (found_it == std::end( clients ))
		return;

	const auto client_id = server.GetClientId( index );

	// NOTE: we do not remove the clients container entry because we could be in the middle of iterating over them.
	// Left over entries will get removed in OnFixedUpdate()

	auto& entry = found_it->second;
	entry.to_be_disconnected = true;

	LOG_INFO( Server, "Client '{}'({}) disconnected", client_id, index );
}

std::pair<YojimboPlugin::ClientIndex_T, GameServer::ClientEntry*> GameServer::FindClientEntry( const Networking::ClientServer::BaseClientConnection& client )
{
	auto [idx, entry] = static_cast<const GameServer*>(this)->FindClientEntry( client ); // laziness to save code duplication
	return std::make_pair( idx, const_cast<ClientEntry*>(entry) );
}

std::pair<YojimboPlugin::ClientIndex_T, const GameServer::ClientEntry*> GameServer::FindClientEntry( const Networking::ClientServer::BaseClientConnection& client ) const
{
	if ((&client.GetOwner() == this) && server.IsClientConnected( client.GetClientIndex() ))
	{
		const auto found_it = clients.find( client.GetClientIndex() );
		if (found_it != std::end( clients ))
			return std::make_pair( found_it->first, &(found_it->second) );
	}

	return std::make_pair( -1, nullptr );
}

void GameServer::DisconnectClient( ClientEntry& entry, std::optional<std::string> reason )
{
	if (entry.to_be_disconnected)
		return;

	NOT_IMPLEMENTED;
	// TODO: send disconnect packet
	entry.to_be_disconnected = true;
}

void GameServer::ProcessMessages()
{
	if (!IsRunning())
		return;

	const auto IsDisconnectedPred = [this]( int idx ) -> bool { return !server.IsClientConnected( idx ); };
	const auto ShouldProcessPred = [&, this]( const ClientContainer_T::const_iterator& it ) -> bool
	{
		const auto& [idx, entry] = *it;

		return !IsDisconnectedPred( idx )
			&& !entry.to_be_disconnected
			&& entry.IsValid()
			;
	};
	const auto ProcessMessageFunc = []( ClientEntry& entry, const yojimbo::Message& message ) -> bool
	{
		if (auto* active_client = std::get_if<ActiveClientPtr>( &entry.client ))
			return (*active_client)->HandleMessage( message );

		else if (auto* unauthed_client = std::get_if<UnauthenticatedClientPtr>( &entry.client ))
			return (*unauthed_client)->HandleMessage( message );

		return false;
	};

	for (int idx = 0, max_clients = server.GetMaxClients(); idx < max_clients; idx++)
	{
		if (!server.IsClientConnected( idx ))
			continue;

		const auto client_it = clients.find( idx );
		if (client_it == std::end( clients ))
		{
			LOG_CRITICAL( Server, "Found a client index without a connection object during ProcessMessages(), severing connection." );
			server.DisconnectClient( idx );
			continue;
		}

		if (!ShouldProcessPred( client_it ))
			continue;

		auto& client_entry = client_it->second;
		for (auto channel : magic_enum::enum_values<Networking::ClientServer::ChannelType>())
		{
			const auto channel_idx = static_cast<YojimboPlugin::ClientIndex_T>(channel);
			yojimbo::Message* message = server.ReceiveMessage( idx, channel_idx );
			while (message != NULL && ShouldProcessPred( client_it ))
			{
				const auto message_type = message->GetType();

				const bool handled = ProcessMessageFunc( client_entry, *message );
				server.ReleaseMessage( idx, message );
				message = NULL;

				if (!handled)
				{
#ifdef _DEBUG
					using namespace std::string_literals;
					DisconnectClient( client_entry, "Unhandled message '"s + std::string{ MessageFactory::GetMessageName( message_type ) } + "'" );
#else
					DisconnectClient( client_entry );
#endif
				}
			}
			// release message in case ShouldProcessPred() returned false
			if (message != NULL)
			{
				server.ReleaseMessage( idx, message );
				message = NULL;
			}
		}
	}
}
