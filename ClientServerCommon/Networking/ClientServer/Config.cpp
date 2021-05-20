#include "Config.hpp"

#include "Common/Utility/MagicEnum.hpp"
#include "Common/Utility/Unreachable.hpp"

const YojimboPlugin::Key_T Networking::ClientServer::DefaultPrivateKey = {
	0xb3, 0xea, 0xdd, 0x4d, 0x45, 0xb7, 0xa2, 0x8b,
	0xdc, 0x63, 0x00, 0xc3, 0x2e, 0x24, 0x63 ,0x2f,
	0x23, 0x0e, 0x6c, 0xa5, 0xa8, 0xaa, 0xd0, 0xc5,
	0x88, 0xe2, 0x45, 0xcd, 0xaa, 0xa6, 0x03, 0x58
};

namespace Networking::ClientServer
{
	yojimbo::ClientServerConfig MakeConfiguration() noexcept
	{
		auto config = yojimbo::ClientServerConfig();

		config.protocolId = 1;
		config.timeout = 6; // seconds
		config.networkSimulator = false;

		config.numChannels = static_cast<int>( ChannelType::NumChannelTypes );
		for (const ChannelType type : magic_enum::enum_values<ChannelType>())
		{
			auto& channel = config.channel[static_cast<size_t>(type)];

			switch (type)
			{
			case ChannelType::Reliable:
				channel.type = yojimbo::ChannelType::CHANNEL_TYPE_RELIABLE_ORDERED;
				break;

			case ChannelType::Unreliable:
				channel.type = yojimbo::ChannelType::CHANNEL_TYPE_UNRELIABLE_UNORDERED;
				break;

			default:
				unreachable();
				break;
			}
		}

		return config;
	}
}
