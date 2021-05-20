#include "MessageFactory.hpp"

namespace Networking::ClientServer
{
	MessageFactory::MessageFactory( yojimbo::Allocator& allocator_ )
		: yojimbo::MessageFactory( allocator_, 0 /* TODO */ )
	{
	}

	MessageFactory::~MessageFactory()
	{

	}

	yojimbo::Message* MessageFactory::CreateMessageInternal( YojimboPlugin::MessageType_T type )
	{
		yojimbo::Allocator& allocator = GetAllocator(); (void)allocator;
		yojimbo::Message* const message = NULL;

		// TODO: allocate the message
		NOT_IMPLEMENTED;

		if (!message)
			return NULL;

		yojimbo::MessageFactory::SetMessageType( message, type );
		return message;
	}
}
