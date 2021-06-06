#include "DedicatedServerGame.hpp"

#include "Common/Core/Core.hpp"

#include "ClientServerCommon/Plugins/PluginTypes.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/YojimboPlugin.hpp"

#include "Server/Game/Server.hpp"


//
// ServerGame
//

DedicatedServerGame::DedicatedServerGame()
{
}

DedicatedServerGame::~DedicatedServerGame()
{
}

void DedicatedServerGame::Init()
{
	server.reset( new Game::Server( *core, yojimbo::Address( "127.0.0.1:42777" ) ) );
	server->GetServer().Start( 8 );
}

void DedicatedServerGame::OnGameEnd()
{
}

void DedicatedServerGame::OnFixedUpdate( const PreciseTimestep& ts )
{
	ASSERT( server != nullptr );
	server->OnFixedUpdate( ts );
}

void DedicatedServerGame::OnVariableUpdate( const PreciseTimestep& ts )
{
	ASSERT( server != nullptr );
	server->OnVariableUpdate( ts );
}
