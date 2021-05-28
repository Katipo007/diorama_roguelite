#pragma once

#include "Helpers/HelperDefines.hpp"

namespace Networking::ClientServer::Messages
{
	DEFINE_EMPTY_MESSAGE( DummyMessage );

	BEGIN_MESSAGE( ServerClientDisconnect )
		std::array<char, 128> reason;
		
		template<typename STREAM>
		bool Serialize( STREAM& stream )
		{
			serialize_string( stream, &reason[0], static_cast<int>(std::size( reason )) );
			return true;
		}
	END_MESSAGE();
}

#include "Helpers/RemoveHelperDefines.hpp"
