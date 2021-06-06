#pragma once

#include "Common/Game/Character/CharacterUtility.hpp"

#include "MessageDefines.hpp"

namespace Game::Networking::Messages
{
	BEGIN_MESSAGE( ClientServerLoginStart )
		std::array<char, Game::CharacterUtility::CharacterNameMaxLength> username;

		template<YojimboPlugin::Concepts::SerializeStream STREAM>
		bool Serialize( STREAM& stream )
		{
			serialize_string( stream, &username[0], static_cast<int>(std::size( username )) );
			return true;
		}
	END_MESSAGE();

	BEGIN_MESSAGE( ServerClientLoginSuccess )
		std::array<char, Game::CharacterUtility::CharacterNameMaxLength> username;
		
		template<YojimboPlugin::Concepts::SerializeStream STREAM>
		bool Serialize( STREAM& stream )
		{
			serialize_string( stream, &username[0], static_cast<int>(std::size( username )) );
			return true;
		}
	END_MESSAGE();
}

#include "MessageUndefines.hpp"
