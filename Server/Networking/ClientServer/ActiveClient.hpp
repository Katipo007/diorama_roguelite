#pragma once

#include "BaseClientConnection.hpp"

namespace Networking::ClientServer
{
	class UnauthenticatedClient;

	class ActiveClient final
		: public BaseClientConnection
	{
	public:
		ActiveClient( UnauthenticatedClient& unauthed_client ); // create an active version of the client object from an unauthed one
		~ActiveClient();

		bool HandleMessage( const yojimbo::Message& message ) override;
	};
}
