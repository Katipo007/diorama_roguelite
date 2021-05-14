#include "Message.hpp"

#include "MessageFactory.hpp"

namespace YojimboPlugin
{
	MessageType Message::GetType() const
	{
		return yojimbo::Message::GetType();
	}

	MessageFactory& Message::GetFactory()
	{
		ASSERT( owner != nullptr );
		return *owner;
	}

	const MessageFactory& Message::GetFactory() const
	{
		ASSERT( owner != nullptr );
		return *owner;
	}
}
