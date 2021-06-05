#pragma once

#include <bitset>

#include "ClientServerCommon/Game/Networking.hpp"
#include "Common/Utility/MagicEnum.hpp"

namespace Game::Components
{
	struct ServerClientConnection final
	{
		Networking::ClientIndex client_index;
		
		enum class Flags : uint8_t
		{
			IsLoopbackClient,
		};
		std::bitset<magic_enum::enum_count<Flags>()> flags;
	};
}
