#pragma once

namespace Game::Networking
{
	struct PendingClientComponent final
	{
		std::string requested_username;
	};
}
