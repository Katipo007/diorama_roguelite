#pragma once

#include "ClientServerCommon/Plugins/Yojimbo/Types.hpp"

namespace Networking::ClientServer
{
	enum class ChannelType : YojimboPlugin::ChannelIndex_T
	{
		Reliable,
		Unreliable,
	};
}
