#include "ServerGame.hpp"

#include "Common/Core/Core.hpp"

#include "ClientServerCommon/Plugins/PluginTypes.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/YojimboPlugin.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/Basic/BasicServer.hpp"
#include "ClientServerCommon/Networking/ClientServerConfig.hpp"
#include "ClientServerCommon/Networking/MessageFactory.hpp"

namespace Game
{
	namespace
	{
		YojimboPlugin::BasicServer::Definition CreateServerDefinition( Plugins::YojimboPlugin& plugin )
		{
			auto message_factory = std::make_shared<Game::Networking::GameMessageFactory>();

			return YojimboPlugin::BasicServer::Definition
			{
				.plugin = &plugin,
				.host_address = "127.0.0.1:42777",
				.private_key = ClientServerConnection::DefaultPrivateKey,
				.max_num_clients = 10,
				.config = ClientServerConnection::MakeConfiguration(),
				.adapter = YojimboPlugin::BasicAdapter( message_factory ),
			};
		}
	}


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

	YojimboPlugin::BaseServer* ServerGame::GetServer()
	{
		return server.get();
	}

	const YojimboPlugin::BaseServer* ServerGame::GetServer() const
	{
		return server.get();
	}

	void ServerGame::Init()
	{
		auto& yojimbo_plugin = core->GetRequiredAPI<Plugins::YojimboPlugin>();

		// initialise server
		server.reset( new YojimboPlugin::BasicServer( CreateServerDefinition( yojimbo_plugin ) ) );
		server->ClientConnected.connect( &ServerGame::ClientConnectedHandler, this );
		server->ClientDisconnected.connect( &ServerGame::ClientDisconnectedHandler, this );
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

	void ServerGame::ClientConnectedHandler( YojimboPlugin::BaseServer&, YojimboPlugin::ClientConnection& client )
	{
		LOG_INFO( Server, "Client '{}' connected at {}", client.GetId(), client.GetConnectedTimestamp() );
	}

	void ServerGame::ClientDisconnectedHandler( YojimboPlugin::BaseServer&, YojimboPlugin::ClientConnection& client )
	{
		LOG_INFO( Server, "Client '{}' disconnected", client.GetId() );
	}
}
