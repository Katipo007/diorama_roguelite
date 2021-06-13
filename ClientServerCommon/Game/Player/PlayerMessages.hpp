#pragma once

#include "ClientServerCommon/Game/Networking/Messages/MessageDefines.hpp"

namespace Game::Player::Messages
{
	/// <summary>
	/// Update a player's position on the server
	/// </summary>
	BEGIN_MESSAGE( ClientServerPlayerPosition )
		float x, y;

		template<typename Stream>
		bool Serialize( Stream& stream )
		{
			serialize_float( stream, x );
			serialize_float( stream, y );
			return true;
		}
	END_MESSAGE();
}

#include "ClientServerCommon/Game/Networking/Messages/MessageUndefines.hpp"
