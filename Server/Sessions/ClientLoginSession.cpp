#include "ClientLoginSession.hpp"

#include "Server/Networking/ClientServer/ServerClientConnection.hpp"

namespace Sessions
{
	ClientLoginSession::ClientLoginSession( std::unique_ptr<Networking::ClientServer::ServerClientConnection> connection_ )
		: BaseServerClientSession( std::move( connection_ ) )
	{
	}

	ClientLoginSession::~ClientLoginSession()
	{
	}

	bool ClientLoginSession::HandleMessage( const yojimbo::Message& )
	{
		return false;
	}
}
