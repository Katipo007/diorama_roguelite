#pragma once

#include <bitset>
#include <chrono>

#include "ClientServerCommon/Game/Networking.hpp"
#include "Common/Utility/MagicEnum.hpp"

namespace Game::Components
{
	struct ServerClientConnection final
	{
		Networking::ClientIndex client_index;
		std::chrono::system_clock::time_point connected_at{ std::chrono::system_clock::now() };
		
		enum class Flags : uint8_t
		{
			IsLoopbackClient,
		};
		std::bitset<magic_enum::enum_count<Flags>()> flags;
	};
}
