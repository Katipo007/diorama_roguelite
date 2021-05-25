#pragma once

#include "BaseServerClientSession.hpp"

namespace Sessions
{
	class ClientLoginSession final
		: public BaseServerClientSession
	{
	public:
		ClientLoginSession( std::unique_ptr<Networking::ClientServer::ServerClientConnection> connection );
		~ClientLoginSession();

	protected:
		bool HandleMessage( const yojimbo::Message& ) override;
	};
}
