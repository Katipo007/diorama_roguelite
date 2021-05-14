#pragma once

#include <memory>
#include <string_view>

#include "Types.hpp"

namespace YojimboPlugin
{
	struct Message;
	class MessageFactory
	{
	public:
		virtual ~MessageFactory() {}

		virtual size_t GetNumMessageTypes() const noexcept = 0;
		virtual bool HasType( const MessageType ) const noexcept = 0;
		virtual Message* CreateUntypedMessage( const MessageType ) = 0;

		virtual std::string_view GetMessageName( const MessageType ) const = 0;
	};
}
