#include "ClientGameSession.hpp"

#include "Client/Networking/ClientServer/ServerConnection.hpp"
#include "ClientServerCommon/Networking/ClientServer/Messages.hpp"

namespace Sessions
{
	ClientGameSession::ClientGameSession( std::unique_ptr<Networking::ClientServer::ServerConnection>& connection_ )
		: BaseClientSession( connection_ )
	{
	}

	ClientGameSession::~ClientGameSession()
	{
	}

	bool ClientGameSession::MessageHandler( Networking::ClientServer::ServerConnection&, const yojimbo::Message& message )
	{
		using namespace Networking::ClientServer;
		using Factory_T = ServerConnection::FactoryType;

#define CASE(message_type) case Factory_T::GetMessageType<message_type>(): { const auto& msg = static_cast<const message_type&>( message );
#define END_CASE break; }

		switch (message.GetType())
		{
			CASE( Messages::ClientServerChatMessage )
				const std::string chat_message = std::string{ msg.message.data() };
				ChatMessageReceived( chat_message );
				return true;
			END_CASE
			
		default:
			return false;
		}

#undef END_CASE
#undef CASE
	}
}
