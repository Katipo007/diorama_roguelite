#include "ActiveClient.hpp"

#include "ClientServerCommon/Networking/ClientServer/MessageFactory.hpp"
#include "Server/GameServer.hpp"
#include "UnauthenticatedClient.hpp"

namespace Networking::ClientServer
{
	ActiveClient::ActiveClient( UnauthenticatedClient& unauthed_client )
		: BaseClientConnection( unauthed_client.GetOwner(), unauthed_client.GetClientIndex() )
	{
		SendMessage<Messages::ServerClientLoginSuccess>( ChannelType::Reliable, []( Messages::ServerClientLoginSuccess& ) {} );
	}
	
	ActiveClient::~ActiveClient() = default;

	bool ActiveClient::HandleMessage( const yojimbo::Message& )
	{
		return false;
	}
}
