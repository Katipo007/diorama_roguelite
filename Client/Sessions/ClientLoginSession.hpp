#pragma once

#include "BaseClientSession.hpp"

namespace Sessions
{
	class ClientLoginSession
		: public BaseClientSession
	{
	public:
		ClientLoginSession( std::unique_ptr<Networking::ClientServer::ServerConnection>& connection );
		virtual ~ClientLoginSession();

	protected:
		bool MessageHandler( Networking::ClientServer::ServerConnection& connection, const yojimbo::Message& message ) override;
	};
}
