#include "ClientLoginSession.hpp"

#include "Client/Networking/ClientServer/ServerConnection.hpp"

namespace Sessions
{
	ClientLoginSession::ClientLoginSession( std::unique_ptr<Networking::ClientServer::ServerConnection>& connection_ )
		: BaseClientSession( connection_ )
	{
	}

	ClientLoginSession::~ClientLoginSession()
	{
	}

	bool ClientLoginSession::MessageHandler( Networking::ClientServer::ServerConnection&, const yojimbo::Message& msg )
	{
		using namespace Networking::ClientServer;
		using F = ServerConnection::FactoryType;

		switch (msg.GetType())
		{
			case F::GetMessageType<DummyMessage>() :
				return false;

			default:
				return false;
		}
	}
}
