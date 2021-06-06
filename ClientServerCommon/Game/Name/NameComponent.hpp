#pragma once

#include <string>
#include "ClientServerCommon/Game/ClientSync/ClientSyncTypes.hpp"

namespace Game::Name
{
	struct NameComponent final
	{
		std::array<char, 128> value;

		void Serialise( typename auto& s )
		{
			s.container1b( value );
		}
	};
}
