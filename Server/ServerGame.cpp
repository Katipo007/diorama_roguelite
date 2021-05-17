#include "ServerGame.hpp"

#include "Common/Core/Core.hpp"

#include "ClientServerCommon/Plugins/PluginTypes.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/YojimboPlugin.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/Basic/BasicServer.hpp"
#include "ClientServerCommon/Networking/ClientServerConfig.hpp"
#include "ClientServerCommon/Networking/MessageFactory.hpp"

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

	YojimboPlugin::Server* ServerGame::GetServer()
	{
		return server.get();
	}

	const YojimboPlugin::Server* ServerGame::GetServer() const
	{
		return server.get();
	}

	void ServerGame::Init()
	{
		// get the network API
		auto& yojimbo = core->GetRequiredAPI<Plugins::YojimboPlugin>(); (void)yojimbo;

		// initialise server
		auto message_factory = std::make_shared<Game::Networking::GameMessageFactory>();
		auto adapter = YojimboPlugin::BasicAdapter( message_factory );
		auto config = ClientServerConnection::MakeConfiguration();
		server.reset( new YojimboPlugin::BasicServer( yojimbo::Address( "127.0.0.1:42777" ), 16, ClientServerConnection::DefaultPrivateKey, std::move( config ), std::move( adapter ) ) );
		server->ClientConnected.connect( &ServerGame::ClientConnectedHandler, this );
		server->ClientDisconnected.connect( &ServerGame::ClientDisconnectedHandler, this );
		yojimbo.Add( *server );
	}

	void ServerGame::OnGameEnd()
	{
		if (server)
		{
			auto& yojimbo = core->GetRequiredAPI<Plugins::YojimboPlugin>();
			yojimbo.Remove( *server );
			server = nullptr;
		}
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

	void ServerGame::ClientConnectedHandler( YojimboPlugin::Server&, YojimboPlugin::ClientConnection& client )
	{
		LOG_INFO( Server, "Client '{}' connected at {}", client.GetId(), client.GetConnectedTimestamp() );
	}

	void ServerGame::ClientDisconnectedHandler( YojimboPlugin::Server&, YojimboPlugin::ClientConnection& client )
	{
		LOG_INFO( Server, "Client '{}' disconnected", client.GetId() );
	}
}
