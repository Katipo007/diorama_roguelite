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

	bool ClientLoginSession::MessageHandler( Networking::ClientServer::ServerConnection&, const yojimbo::Message& message )
	{
		using namespace Networking::ClientServer;
		using Factory = ServerConnection::FactoryType;

		switch (message.GetType())
		{
			case Factory::GetMessageType<Messages::DummyMessage>() :
				return false;

			default:
				return false;
		}
	}
}
