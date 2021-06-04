#pragma once

#include <bitset>

#include "ClientServerCommon/ecs.hpp"
#include "Common/Utility/MagicEnum.hpp"

namespace Game::Networking
{
	struct ActiveClientState final
	{
		enum class Flags : uint8_t
		{
			Disconnecting = 0, // Whether this connection is going to be disconnected
		};
		std::bitset<magic_enum::enum_count<Flags>()> flags;

		ecs::EntityHandle player;
	};
}
