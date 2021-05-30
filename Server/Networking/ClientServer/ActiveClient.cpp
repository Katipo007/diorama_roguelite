#include "ActiveClient.hpp"

#include "ClientServerCommon/Networking/ClientServer/MessageFactory.hpp"
#include "GameServer.hpp"
#include "UnauthenticatedClient.hpp"

#include "Common/Utility/StringUtility.hpp"

namespace Networking::ClientServer
{
	ActiveClient::ActiveClient( UnauthenticatedClient& unauthed_client )
		: BaseClientConnection( unauthed_client.GetOwner(), unauthed_client.GetClientIndex() )
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
				LOG_INFO( Server, "Chat message received from ({}): {}", GetClientIndex(), chat.message.data() );

				owner.BroadcastMessage<Messages::ServerClientChatMessage>( ChannelType::Reliable, [&chat]( ActiveClient&, Messages::ServerClientChatMessage& outgoing )
					{
						StringUtility::StringToArray( "UNKNOWN", outgoing.sender );
						StringUtility::StringToArray( chat.message.data(), outgoing.message );
					} );
				return true;
				break;
			}
		}

		return false;
	}
}
