#pragma once

#include "ClientServerCommon/Vendor/Wrappers/Networking.hpp"


#define GENERATE_MESSAGES_IMPL
#define REGISTER_CLIENT_SERVER_MESSAGE( name, comment ) 
#define REGISTER_SERVER_CLIENT_MESSAGE( name, comment ) 

namespace Networking::Message
{
#include "Messages.def"
}

#undef REGISTER_CLIENT_SERVER_MESSAGE
#undef REGISTER_SERVER_CLIENT_MESSAGE
#undef GENERATE_MESSAGES_IMPL
