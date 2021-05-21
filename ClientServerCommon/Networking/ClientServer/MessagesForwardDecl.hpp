#pragma once

#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"

#define DEFINE_MESSAGE( name, members, methods, serialise ) struct name;

namespace Networking::ClientServer
{
	#include "Messages.def"
}

#undef DEFINE_MESSAGE
