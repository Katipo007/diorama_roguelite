#pragma once

#include "Common/Core/Base.hpp"
#include "ClientServerCommon/Vendor/Wrappers/Networking.hpp"

#include "MessageId.hpp"
#include "Messages.hpp"

namespace Networking
{
	// the message factory
	YOJIMBO_MESSAGE_FACTORY_START( ClientServerGameMessageFactory, MessageId::NumMessageIds );

#define REGISTER_CLIENT_SERVER_MESSAGE( name, comment ) YOJIMBO_DECLARE_MESSAGE_TYPE( ::Networking::MessageId::EXPAND_MACRO( name )Id, ::Networking::Message::EXPAND_MACRO( name ) );
#define REGISTER_SERVER_CLIENT_MESSAGE( name, comment ) YOJIMBO_DECLARE_MESSAGE_TYPE( ::Networking::MessageId::EXPAND_MACRO( name )Id, ::Networking::Message::EXPAND_MACRO( name ) );

#include "Messages.def"

#undef REGISTER_CLIENT_SERVER_MESSAGE
#undef REGISTER_SERVER_CLIENT_MESSAGE

	YOJIMBO_MESSAGE_FACTORY_FINISH();
}
