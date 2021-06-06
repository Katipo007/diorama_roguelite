#pragma once

#include "MessageDefines.hpp"

namespace Game::Networking::Messages
{
	DEFINE_EMPTY_MESSAGE( DummyMessage );

	BEGIN_MESSAGE( ServerClientDisconnect )
		std::string reason;
		
		template<YojimboPlugin::Concepts::SerializeStream STREAM>
		bool Serialize( STREAM& stream )
		{
			serialize_string( stream, reason.data(), 128 );
			return true;
		}
	END_MESSAGE();
}

#include "MessageUndefines.hpp"
