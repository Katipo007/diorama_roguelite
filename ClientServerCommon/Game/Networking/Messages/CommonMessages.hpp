#pragma once

#include "MessageDefines.hpp"

namespace Game::Networking::Messages
{
	DEFINE_EMPTY_MESSAGE( DummyMessage );

	BEGIN_MESSAGE( ServerClientDisconnect )
		std::array<char, 128> reason;
		
		template<YojimboPlugin::Concepts::SerializeStream STREAM>
		bool Serialize( STREAM& stream )
		{
			serialize_string( stream, &reason[0], static_cast<int>(std::size( reason )) );
			return true;
		}
	END_MESSAGE();
}

#include "MessageUndefines.hpp"
