#include "ClientServerConfig.hpp"

namespace Networking
{
	const uint8_t ClientServerGameConnectionConfig::DefaultPrivateKey[yojimbo::KeyBytes] = {
			0xb3, 0xea, 0xdd, 0x4d, 0x45, 0xb7, 0xa2, 0x8b,
			0xdc, 0x63, 0x00, 0xc3, 0x2e, 0x24, 0x63 ,0x2f,
			0x23, 0x0e, 0x6c, 0xa5, 0xa8, 0xaa, 0xd0, 0xc5,
			0x88, 0xe2, 0x45, 0xcd, 0xaa, 0xa6, 0x03, 0x58
	};


	ClientServerGameConnectionConfig::ClientServerGameConnectionConfig()
		: yojimbo::ClientServerConfig()
	{
		numChannels = 2;
		channel[ChannelType::Reliable].type = yojimbo::CHANNEL_TYPE_RELIABLE_ORDERED;
		channel[ChannelType::Unreliable].type = yojimbo::CHANNEL_TYPE_UNRELIABLE_UNORDERED;

		networkSimulator = false;
	}
}