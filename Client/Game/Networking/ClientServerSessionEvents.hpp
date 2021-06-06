#pragma once

#include <string>

namespace Game::Networking::ClientServerSessionEvents
{
	struct ChatMessageReceived final
	{
		std::string sender;
		std::string message;
	};
}
