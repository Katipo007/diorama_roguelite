#pragma once

#include "ClientServerCommon/Game/Networking.hpp"

namespace Game::Networking
{
	struct ServerConnectionRequest
	{
		std::string destination;
		std::string username;
		ClientId client_id;
	};
}
