#pragma once

#include "ClientServerCommon/Plugins/PluginTypes.hpp"

namespace ServerPlugins
{
	enum Type : APIType
	{
		NumServerPlugins = ClientServerCommonPlugins::NumClientServerCommonPlugins
	};
}
