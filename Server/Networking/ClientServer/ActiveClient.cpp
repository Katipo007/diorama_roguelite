#include "ActiveClient.hpp"

#include "ClientServerCommon/Networking/ClientServer/MessageFactory.hpp"
#include "GameServer.hpp"
#include "UnauthenticatedClient.hpp"

#include "Common/Utility/StringUtility.hpp"

namespace Networking::ClientServer
{
	ActiveClient::ActiveClient( UnauthenticatedClient& unauthed_client )
		: BaseClientConnection( unauthed_client.GetOwner(), unauthed_client.GetClientIndex() )
		, username( unauthed_client.GetUsername() )
	{
		SendMessage<Messages::ServerClientLoginSuccess>( ChannelType::Reliable, []( Messages::ServerClientLoginSuccess& ) {} );
	}
	
	ActiveClient::~ActiveClient() = default;

	bool ActiveClient::HandleMessage( const yojimbo::Message& incoming )
	{
		switch (incoming.GetType())
		{
			case MessageFactory::GetMessageType<Messages::ClientServerChatMessage>() :
			{
				auto& chat = static_cast<const Messages::ClientServerChatMessage&>(incoming);
				LOG_INFO( LoggingChannels::Server, "Chat message received from '{}'({}): {}", username.c_str(), GetClientIndex(), chat.message.data() );

				owner.BroadcastMessage<Messages::ServerClientChatMessage>( ChannelType::Reliable, [&chat, this]( ActiveClient&, Messages::ServerClientChatMessage& outgoing )
					{
						StringUtility::StringToArray( username, outgoing.sender );
						StringUtility::StringToArray( chat.message.data(), outgoing.message );
					} );
				return true;
				break;
			}
		}

		return false;
	}
}
