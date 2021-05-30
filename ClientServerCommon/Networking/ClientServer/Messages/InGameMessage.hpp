#pragma once

#include "Helpers/HelperDefines.hpp"

namespace Networking::ClientServer::Messages
{
	BEGIN_MESSAGE( ClientServerChatMessage )
		std::array<char, 128> message;
		
		template<typename STREAM>
		bool Serialize( STREAM& stream )
		{
			serialize_string( stream, &message[0], static_cast<int>(std::size( message )) );
			return true;
		}
	END_MESSAGE();

	BEGIN_MESSAGE( ServerClientChatMessage )
		std::array<char, 32> sender;
		std::array<char, 128> message;

	template<typename STREAM>
	bool Serialize( STREAM& stream )
	{
		serialize_string( stream, &sender[0], static_cast<int>(std::size( sender )) );
		serialize_string( stream, &message[0], static_cast<int>(std::size( message )) );
		return true;
	}
	END_MESSAGE();
}

#include "Helpers/RemoveHelperDefines.hpp"
