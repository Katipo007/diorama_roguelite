#include "BaseClientConnection.hpp"

#include "Server/GameServer.hpp"

namespace Networking::ClientServer
{
	BaseClientConnection::BaseClientConnection( GameServer& owner, YojimboPlugin::ClientIndex_T client_index )
		: owner( owner )
		, client_index( client_index )
	{

	}

	BaseClientConnection::~BaseClientConnection() = default;
}
