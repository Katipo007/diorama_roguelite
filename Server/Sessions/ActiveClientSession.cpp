#include "ActiveClientSession.hpp"

#include "ClientServerCommon/Networking/MessageId.hpp"
#include "ClientServerCommon/Networking/Messages.hpp"

namespace Server::Sessions
{
	ActiveClientSession::ActiveClientSession( GameServer& server, const int client_index, const uint64_t client_id )
		: server( server )
		, client_index( client_index )
		, client_id( client_id )
		, SessionFlags( (Flags)0 )
	{
		LOG_INFO( Server, "Client {} connected", client_id );
	}

	ActiveClientSession::~ActiveClientSession()
	{
		LOG_INFO( Server, "Client {} disconnected", client_id );
	}

	void ActiveClientSession::Update( const PreciseTimestep& timestep )
	{
		(void)timestep;
	}

	bool ActiveClientSession::ProcessMessage( const yojimbo::Message& message )
	{
		switch (message.GetType())
		{
		case Networking::MessageId::ChatMessageSBId:
		{
			const auto& chat_msg = static_cast<const Networking::Message::ChatMessageSB&>(message);

			LOG_INFO( Server, "Chat message received from client '{}': '{}'", client_id, chat_msg.message );

			// TODO: process message for commands
			// TODO: pass message on to other clients
		}
		break;

		}

		return false;
	}
}