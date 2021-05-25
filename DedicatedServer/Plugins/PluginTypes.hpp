#pragma once

#include "ClientServerCommon/Plugins/PluginTypes.hpp"

namespace DedicatedServerPlugins
{
	enum Type : APIType
	{
		NumDedicatedServerPlugins = ClientServerCommonPlugins::NumClientServerCommonPlugins
	};
}
