#pragma once

#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/Types.hpp"

#include "Channels.hpp"

namespace Game::Networking
{
	extern const YojimboPlugin::Key_T DefaultPrivateKey;

	yojimbo::ClientServerConfig MakeConfiguration() noexcept;
}
