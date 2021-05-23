#pragma once

#include "BaseServerClientSession.hpp"

namespace Sessions
{
	class ClientActiveSession final
		: public BaseServerClientSession
	{
	public:
		ClientActiveSession( std::unique_ptr<Networking::ClientServer::ServerClientConnection> connection );
		~ClientActiveSession();

	protected:
		bool HandleMessage( const yojimbo::Message& ) override;
	};
}
