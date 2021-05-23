#include "GameServer.hpp"

#include "ClientServerCommon/Networking/ClientServer/Config.hpp"

#include "Server/Networking/ClientServer/ServerClientConnection.hpp"
#include "Server/Sessions/BaseServerClientSession.hpp"
#include "Server/Sessions/ClientLoginSession.hpp"
#include "Server/Sessions/ClientActiveSession.hpp"

bool GameServer::ClientEntry::Expired() const
{
	return !session || session->Expired();
}

GameServer::ClientEntry::ClientEntry( std::unique_ptr<Sessions::BaseServerClientSession> session_ )
	: session( std::move( session_ ) )
{
}

GameServer::GameServer( yojimbo::Address host_address, const int max_clients )
	: yojimbo::Server( yojimbo::GetDefaultAllocator(), Networking::ClientServer::DefaultPrivateKey.data(), host_address, Networking::ClientServer::MakeConfiguration(), adapter, 0.0 )
	, adapter()
{
	// Reserve memory
	clients.reserve( max_clients );

	// Connect listeners
	adapter.ServerClientConnected.connect( &GameServer::ClientConnectedHandler, this );
	adapter.ServerClientDisconnected.connect( &GameServer::ClientDisconnectedHandler, this );

	// Start the server
	yojimbo::Server::Start( max_clients );
	std::array<char, 256> server_address_buffer{ "\0" };
	GetAddress().ToString( &server_address_buffer[0], static_cast<int>(std::size( server_address_buffer )) );

	if (!IsRunning())
		throw std::runtime_error( "Could not start server on '" + std::string( server_address_buffer.data() ) + "'" );
}

GameServer::~GameServer()
{
	if (IsRunning())
		Stop();

	ASSERT( !IsRunning() );
}

void GameServer::OnFixedUpdate( const PreciseTimestep& ts )
{
	if (!IsRunning())
	{
		// TODO: should this throw? Do we want the server to be a state where it isn't running??
	}
	else
	{
		yojimbo::Server::ReceivePackets();
		
		// Process clients
		for (auto& [id, entry] : clients)
		{
			if (entry.Expired())
				continue;

			entry.session->OnFixedUpdate( ts );
		}

		yojimbo::Server::SendPackets();

		// Disconnect clients
		std::erase_if( clients, []( const ClientContainer_T::value_type& entry ) { return entry.second.to_be_disconnected || entry.second.Expired(); } );
	}

	yojimbo::Server::AdvanceTime( yojimbo::Server::GetTime() + ts.delta );
}

void GameServer::ClientConnectedHandler( Networking::ClientServer::ClientServerAdapter&, YojimboPlugin::ClientIndex_T index )
{
	const auto client_id = yojimbo::Server::GetClientId( index );
	if (clients.contains( index ))
	{
		LOG_WARN( Server, "Already have a connected client with id '{}', disconnecting new client ({})", client_id, index );
		yojimbo::Server::DisconnectClient( index );
		return;
	}

	auto connection = std::make_unique<Networking::ClientServer::ServerClientConnection>( static_cast<yojimbo::Server&>( *this ), index );
	auto session = std::make_unique<Sessions::ClientLoginSession>( std::move( connection ) );

	const auto [it, success] = clients.try_emplace( index, std::move( session ) );
	if (!success)
	{
		LOG_WARN( Server, "Failed to create new session for client '{}'({})", client_id, index );
		yojimbo::Server::DisconnectClient( index );
		return;
	}
	LOG_INFO( Server, "Client '{}'({}) connected", client_id, index );
	ASSERT( !it->second.Expired() );
}

void GameServer::ClientDisconnectedHandler( Networking::ClientServer::ClientServerAdapter&, YojimboPlugin::ClientIndex_T index )
{
	const auto found_it = clients.find( index );
	if (found_it == std::end( clients ))
		return;

	const auto client_id = yojimbo::Server::GetClientId( index );
	std::unique_ptr<Networking::ClientServer::ServerClientConnection> connection;

	// NOTE: we do not remove the clients container entry because we could be in the middle of iterating over them.
	// Left over entries will get removed in OnFixedUpdate()

	auto& entry = found_it->second;
	entry.to_be_disconnected = true;
	// release the connection from the session
	if (!!entry.session)
	{
		connection = entry.session->ReleaseConnection();
		entry.session.reset();
	}

	LOG_INFO( Server, "Client '{}'({}) Disconnected", client_id, index );

	// destroy the connection object
	if (!!connection)
	{
		connection->being_destroyed_from_disconnect_handler = true; // set this flag so the destructor doesn't try to call yojimbo::Server::DisconnectClient()
		connection.reset();
	}
}
