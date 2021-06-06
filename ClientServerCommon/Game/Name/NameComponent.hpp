#pragma once

#include <string>
#include "ClientServerCommon/Game/ClientSync/ClientSyncTypes.hpp"

namespace Game::Name
{
	struct NameComponent final
	{
		std::array<char, 128> value;

		ClientSync::OComponentDataStream& Serialise( ClientSync::OComponentDataStream& stream ) const
		{
			const auto len = static_cast<uint8_t>(std::size( value ));
			stream << len << 

			return stream;
		}
	};
}
