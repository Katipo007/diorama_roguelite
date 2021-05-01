#pragma once

namespace Networking
{
	enum class ClientConnectionState : unsigned
	{
		Default,
		Authenticated,
		Accepted,
		Disconnected,

		NumStates
	};
}
