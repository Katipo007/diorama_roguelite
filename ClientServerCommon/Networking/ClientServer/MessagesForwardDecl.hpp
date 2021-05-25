#pragma once

#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"

#define DEFINE_MESSAGE( name, members, methods, serialise ) struct name;

namespace Networking::ClientServer::Messages
{
	#include "Messages.def"
}

#undef DEFINE_MESSAGE
