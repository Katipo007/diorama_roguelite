#include "ServerGame.hpp"

#include "Common/Core/Core.hpp"

#include "ClientServerCommon/Plugins/PluginTypes.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/YojimboPlugin.hpp"

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
	}

	void ServerGame::OnGameEnd()
	{
	}

	void ServerGame::OnFixedUpdate( const PreciseTimestep& ts )
	{
		(void)ts;
		// TODO: call Exit() if server is stopped
	}

	void ServerGame::OnVariableUpdate( const PreciseTimestep& ts )
	{
		(void)ts;
	}
}
