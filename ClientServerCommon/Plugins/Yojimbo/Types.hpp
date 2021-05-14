#pragma once

#include <array>
#include <string>
#include <cinttypes>

namespace YojimboPlugin
{
	using MessageType = int;

	using Address = std::string;

	using ClientId_T = uint64_t;
	using Key_T = std::array<uint8_t, 32>;

	enum class ChannelType : unsigned
	{
		Reliable,
		Unreliable,

		NumChannelTypes
	};

	class ClientConnection;
	struct Message;
	using MessageInitialiserFunc_T = std::function<void( ClientConnection&, Message& )>;
}
