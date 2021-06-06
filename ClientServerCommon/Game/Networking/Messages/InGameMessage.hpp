#pragma once

#include "MessageDefines.hpp"

namespace Game::Networking::Messages
{
	BEGIN_MESSAGE( ClientServerChatMessage )
		std::array<char, 128> message;
		
		template<YojimboPlugin::Concepts::SerializeStream STREAM>
		bool Serialize( STREAM& stream )
		{
			serialize_string( stream, &message[0], static_cast<int>(std::size( message )) );
			return true;
		}
	END_MESSAGE();

	BEGIN_MESSAGE( ServerClientChatMessage )
		std::string sender;
		std::string message;

	template<YojimboPlugin::Concepts::SerializeStream STREAM>
	bool Serialize( STREAM& stream )
	{
		serialize_string( stream, sender.data(), 32 );
		serialize_string( stream, message.data(), 128 );
		return true;
	}
	END_MESSAGE();
}

#include "MessageUndefines.hpp"
