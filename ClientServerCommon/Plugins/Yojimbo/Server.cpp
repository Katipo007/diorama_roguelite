#include "Server.hpp"

#include "ClientConnection.hpp"
#include "Message.hpp"

namespace YojimboPlugin
{
	bool BaseServer::HandleMessage( ClientConnection& connection, Message& message )
	{
		if (connection.message_handler)
			return connection.message_handler( message );

		return false;
	}
}
