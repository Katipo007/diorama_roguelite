#include "ActiveClientSession.hpp"

#include "Server/GameServer.hpp"

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
	}

	ActiveClientSession::~ActiveClientSession()
	{
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
			
			// pass message along to other clients
			{
				const auto chat_message = std::string{ chat_msg.message };
				server.BroadcastMessage( Networking::MessageId::ChatMessageCBId, Networking::ChannelType::Unreliable, [this, chat_message]( yojimbo::Message& msg, const Server::Sessions::ActiveClientSession& )
					{
						auto& p = dynamic_cast<Networking::Message::ChatMessageCB&>(msg);
						strcpy_s( p.sender, display_name.c_str() );
						strcpy_s( p.message, chat_message.c_str() );
					} );
			}
		}
		break;

		}

		return false;
	}
}
