#pragma once

#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/Types.hpp"

namespace Networking::ClientServer
{
	extern const YojimboPlugin::Key_T DefaultPrivateKey;

	enum class ChannelType : YojimboPlugin::ChannelIndex_T
	{
		Reliable,
		Unreliable,

		NumChannelTypes
	};

	yojimbo::ClientServerConfig MakeConfiguration() noexcept;
}
