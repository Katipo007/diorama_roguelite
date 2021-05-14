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
		ASSERT( server );
		return server.get();
	}

	const YojimboPlugin::Server* ServerGame::GetServer() const
	{
		ASSERT( server );
		return server.get();
	}

	void ServerGame::Init()
	{
		// get the network API
		auto* network = core->GetAPI<Plugins::YojimboPlugin>();
		if (!network)
			throw std::runtime_error( "Yojimbo plugin not initialised" );

		// initialise server
		server.reset( new YojimboPlugin::BasicServer( yojimbo::Address( "127.0.0.1:42777" ), 16, ClientServerConnection::DefaultPrivateKey, {}, YojimboPlugin::BasicAdapter( std::make_shared<Game::Networking::GameMessageFactory>() ) ) );
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
