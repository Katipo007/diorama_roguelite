#pragma once

#include <string>
#include "ClientServerCommon/Game/ClientSync/ClientSyncTypes.hpp"

namespace Game::Name
{
	struct NameComponent final
	{
		static constexpr std::size_t MaxNameLength = 48;
		std::string value;

		void Serialise( typename auto& s )
		{
			s.text1b( value, MaxNameLength );
		}
	};
}
