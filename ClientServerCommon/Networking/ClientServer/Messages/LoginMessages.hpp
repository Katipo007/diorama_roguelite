#pragma once

#include "Common/Game/Character/CharacterUtility.hpp"

#include "Helpers/HelperDefines.hpp"

namespace Networking::ClientServer::Messages
{
	BEGIN_MESSAGE( ServerClientLoginSuccess )
		std::array<char, Game::CharacterUtility::CharacterNameMaxLength> username;
		
		template<typename STREAM>
		bool Serialize( STREAM& stream )
		{
			serialize_string( stream, &username[0], static_cast<int>(std::size( username )) );
			return true;
		}
	END_MESSAGE();
}

#include "Helpers/RemoveHelperDefines.hpp"
