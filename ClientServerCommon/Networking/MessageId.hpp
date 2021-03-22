#pragma once
#include "Common/Core/Base.hpp"

namespace Networking::MessageId
{
	using MessageId_T = uint16_t;
	enum MessageId : MessageId_T
	{
#define REGISTER_CLIENT_SERVER_MESSAGE( message_name, comment ) EXPAND_MACRO( message_name )Id,
#define REGISTER_SERVER_CLIENT_MESSAGE( message_name, comment ) EXPAND_MACRO( message_name )Id,

#include "Messages.def"

#undef REGISTER_CLIENT_SERVER_MESSAGE
#undef REGISTER_SERVER_CLIENT_MESSAGE
		NumMessageIds
	};
	static_assert((size_t)NumMessageIds > 0, "MessageId generator error");
	static_assert((size_t)NumMessageIds < std::numeric_limits<MessageId_T>::max(), "Too many message types for current implementation!");
}
