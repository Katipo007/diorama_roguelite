#pragma once

#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/Types.hpp"

#include "Channels.hpp"

namespace Networking::ClientServer
{
	extern const YojimboPlugin::Key_T DefaultPrivateKey;

	yojimbo::ClientServerConfig MakeConfiguration() noexcept;
}
