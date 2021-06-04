#pragma once

#include "ClientServerCommon/Plugins/Yojimbo/Types.hpp"

namespace Game::Networking
{
	enum class ChannelType : YojimboPlugin::ChannelIndex_T
	{
		Reliable,
		Unreliable,
	};
}
