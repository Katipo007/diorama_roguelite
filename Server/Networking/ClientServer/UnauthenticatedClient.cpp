#include "UnauthenticatedClient.hpp"

#include "Networking/ClientServer/MessageFactory.hpp"
#include "GameServer.hpp"

namespace Networking::ClientServer
{
	UnauthenticatedClient::UnauthenticatedClient( GameServer& owner, YojimboPlugin::ClientIndex_T client_index )
		: BaseClientConnection( owner, client_index )
	{
	}

	UnauthenticatedClient::~UnauthenticatedClient() = default;

	bool UnauthenticatedClient::HandleMessage( const yojimbo::Message& message )
	{
		switch (message.GetType())
		{
			case MessageFactory::GetMessageType<Messages::ClientServerLoginStart>() :
			{
				const auto& login_start = static_cast<const Messages::ClientServerLoginStart&>(message);
				auto username = std::string{ login_start.username.data() };
				LOG_INFO( Server, "Got login start message from connection '{}' with username '{}'", client_index, username );
				owner.AcceptClient( *this );
				return true;
			}
		}

		return false;
	}
}
