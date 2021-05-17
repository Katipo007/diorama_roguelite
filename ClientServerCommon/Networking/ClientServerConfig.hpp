#pragma once

#include "ClientServerCommon/Plugins/Yojimbo/Types.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/impl/YojimboHeader.hpp"

namespace Game::ClientServerConnection
{
	extern const YojimboPlugin::Key_T DefaultPrivateKey;

	yojimbo::ClientServerConfig MakeConfiguration();
}
