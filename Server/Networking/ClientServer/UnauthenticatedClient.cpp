#include "UnauthenticatedClient.hpp"

#include "GameServer.hpp"
#include "ClientServerCommon/Networking/ClientServer/MessageFactory.hpp"

#include "Common/Game/Character/CharacterUtility.hpp"
#include "Common/Utility/StringUtility.hpp"

namespace
{
	bool IsValidUsername( std::string_view username )
	{
		const auto length = username.length();
		return
			(length >= Game::CharacterUtility::CharacterNameMinLength)
			&& (length <= Game::CharacterUtility::CharacterNameMaxLength)
			;
	}
}

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
				requested_username = login_start.username.data();
				LOG_INFO( Server, "Got login start message from connection '{}' with username '{}'", client_index, requested_username );

				if (IsValidUsername( requested_username ))
					owner.AcceptClient( *this );
				else
					owner.RejectClient( *this );
				return true;
			}
		}

		return false;
	}

	std::string_view UnauthenticatedClient::GetUsername() const
	{
		return requested_username;
	}
}
