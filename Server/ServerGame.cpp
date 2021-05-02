#include "ServerGame.hpp"

#include "Common/Core/Core.hpp"
#include "Common/Core/API/NetworkAPI.hpp"
#include "Common/Networking/Server.hpp"

#include "ClientServerCommon/Networking/ClientServerConfig.hpp"

namespace Game
{
	struct ServerGame::Data
	{
		// TODO
	};


	//
	// ServerGame
	//

	ServerGame::ServerGame()
		: data( std::make_unique<Data>() )
	{
	}

	ServerGame::~ServerGame()
	{
		data.reset();
	}

	::Networking::Server* ServerGame::GetServer()
	{
		ASSERT( server );
		return server.get();
	}

	const::Networking::Server* ServerGame::GetServer() const
	{
		ASSERT( server );
		return server.get();
	}

	void ServerGame::Init()
	{
		// get the network API
		auto* network = core->GetAPI<API::NetworkAPI>();
		if (!network)
			throw std::runtime_error( "No network library initalised" );

		// initialise server
		{
			::Networking::ServerProperties properties;
			properties.host_address = ::Networking::Address( "127.0.0.1:42777" );
			properties.max_num_clients = 16;
			properties.private_key = ClientServerConnection::DefaultPrivateKey;
			server = network->CreateServer( std::move( properties ) );
		}
	}

	void ServerGame::OnGameEnd()
	{
		server.reset();
	}

	void ServerGame::OnFixedUpdate( const PreciseTimestep& ts )
	{
		(void)ts;
		if (!server || !server->IsRunning())
			Exit( 0 );
	}

	void ServerGame::OnVariableUpdate( const PreciseTimestep& ts )
	{
		(void)ts;
	}
}
