#include "ServerGame.hpp"

#include "Common/Core/Core.hpp"

#include "ClientServerCommon/Plugins/PluginTypes.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/YojimboPlugin.hpp"

#include "Server/Networking/ClientServer/GameServer.hpp"

namespace Game
{
	struct ServerGame::Pimpl
	{
		// TODO
	};


	//
	// ServerGame
	//

	ServerGame::ServerGame()
		: pimpl( std::make_unique<Pimpl>() )
	{
	}

	ServerGame::~ServerGame()
	{
	}

	void ServerGame::Init()
	{
		server.reset( new Networking::ClientServer::GameServer( yojimbo::Address( "127.0.0.1:42777" ), 8 ) );
	}

	void ServerGame::OnGameEnd()
	{
	}

	void ServerGame::OnFixedUpdate( const PreciseTimestep& ts )
	{
		if (!server || !server->IsRunning())
		{
			Exit( 0 );
			return;
		}
		
		server->OnFixedUpdate( ts );
	}

	void ServerGame::OnVariableUpdate( const PreciseTimestep& ts )
	{
		(void)ts;
	}
}
