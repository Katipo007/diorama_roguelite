#pragma once

#include <bitset>
#include <chrono>

#include "ClientServerCommon/Game/Networking.hpp"
#include "Common/Utility/MagicEnum.hpp"

namespace yojimbo { class Server; }

namespace Game::Networking
{
	struct ConnectionComponent final
	{
		ConnectionComponent( yojimbo::Server& owner, Networking::ClientIndex client_index )
			: owner{ &owner }, client_index{ client_index }
		{}

		yojimbo::Server& GetOwner() { ASSERT( owner != nullptr ); return *owner; }


		Networking::ClientIndex client_index;
		std::chrono::system_clock::time_point connected_at{ std::chrono::system_clock::now() };
		
		enum class Flags : uint8_t
		{
			IsLoopbackClient,
		};
		std::bitset<magic_enum::enum_count<Flags>()> flags;


	private:
		yojimbo::Server* owner = nullptr;
	};
}
