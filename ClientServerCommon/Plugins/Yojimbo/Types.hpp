#pragma once

#include <array>
#include <cinttypes>
#include <concepts>

namespace yojimbo { class Message; }

namespace YojimboPlugin
{
	using MessageType_T = int;
	using ClientIndex_T = int;
	using ClientId_T = uint64_t;
	using Key_T = std::array<uint8_t, 32>;
	using ChannelIndex_T = int;
}
