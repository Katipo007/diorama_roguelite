#include "ServerGame.hpp"

namespace Game
{
	struct ServerGame::ServerData
	{
		// TODO
	};


	//
	// ServerGame
	//

	ServerGame::ServerGame()
		: data( std::make_unique<ServerData>() )
	{
		NOT_IMPLEMENTED;
	}

	ServerGame::~ServerGame()
	{
		data.reset();
		NOT_IMPLEMENTED;
	}

	void ServerGame::Init()
	{
		NOT_IMPLEMENTED;
	}

	void ServerGame::OnGameEnd()
	{
		NOT_IMPLEMENTED;
	}

	void ServerGame::OnFixedUpdate( const PreciseTimestep& ts )
	{
		(void)ts;
		NOT_IMPLEMENTED;
	}

	void ServerGame::OnVariableUpdate( const PreciseTimestep& ts )
	{
		(void)ts;
		NOT_IMPLEMENTED;
	}
}
