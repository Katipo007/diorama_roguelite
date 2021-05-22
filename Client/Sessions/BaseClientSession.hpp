#pragma once

#include <memory>

#include "Common/Utility/Timestep.hpp"

namespace yojimbo { class Message; }
namespace Networking::ClientServer { class ServerConnection; }

namespace Sessions
{
	class BaseClientSession
	{
	public:
		BaseClientSession( std::unique_ptr<Networking::ClientServer::ServerConnection>& connection );
		virtual ~BaseClientSession();

		bool Expired() const;
		[[nodiscard]] std::unique_ptr<Networking::ClientServer::ServerConnection> ReleaseConnection();

		virtual void OnFixedUpdate( const PreciseTimestep& ts );

	protected:
		virtual bool MessageHandler( Networking::ClientServer::ServerConnection& connection, const yojimbo::Message& message );
		
		Networking::ClientServer::ServerConnection* GetConnection();
		const Networking::ClientServer::ServerConnection* GetConnection() const;

	private:
		BaseClientSession( const BaseClientSession& ) = delete;
		BaseClientSession( BaseClientSession&& ) = delete;
		BaseClientSession operator=( const BaseClientSession& ) = delete;
		BaseClientSession operator=( BaseClientSession&& ) = delete;

		std::unique_ptr<Networking::ClientServer::ServerConnection> connection;
	};
}
