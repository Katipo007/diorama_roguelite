#pragma once

#include <memory>
#include "Common/Utility/Timestep.hpp"

namespace yojimbo { class Message; }
namespace Networking::ClientServer { class ServerClientConnection; }

namespace Sessions
{
	class BaseServerClientSession
	{
	public:
		BaseServerClientSession( std::unique_ptr<Networking::ClientServer::ServerClientConnection> connection );
		virtual ~BaseServerClientSession();

		bool Expired() const noexcept;
		[[nodiscard]] std::unique_ptr<Networking::ClientServer::ServerClientConnection> ReleaseConnection();

		virtual void OnFixedUpdate( const PreciseTimestep& ts );

	protected:
		virtual bool HandleMessage( const yojimbo::Message& message ) = 0;

		Networking::ClientServer::ServerClientConnection* GetConnection();
		const Networking::ClientServer::ServerClientConnection* GetConnection() const;

	private:
		bool InternalMessageHandler( Networking::ClientServer::ServerClientConnection& connection, const yojimbo::Message& message );

		BaseServerClientSession( const BaseServerClientSession& ) = delete;
		BaseServerClientSession( BaseServerClientSession&& ) = delete;
		BaseServerClientSession operator=( const BaseServerClientSession& ) = delete;
		BaseServerClientSession operator=( BaseServerClientSession&& ) = delete;

	private:
		std::unique_ptr<Networking::ClientServer::ServerClientConnection> connection;
	};
}
