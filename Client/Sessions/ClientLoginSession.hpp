#pragma once

#include <memory>

namespace yojimbo { class Message; }
namespace Networking::ClientServer { class ServerConnection; }

namespace Sessions
{
	class ClientLoginSession
	{
		using Connection_T = std::unique_ptr<Networking::ClientServer::ServerConnection>;

	public:
		ClientLoginSession( Connection_T&& connection );
		virtual ~ClientLoginSession();

		bool Expired() const;
		[[nodiscard]] Connection_T ReleaseConnection();

	protected:
		virtual bool MessageHandler( Networking::ClientServer::ServerConnection& connection, const yojimbo::Message& message );

	private:
		ClientLoginSession( const ClientLoginSession& ) = delete;
		ClientLoginSession( ClientLoginSession&& ) = delete;
		ClientLoginSession operator=( const ClientLoginSession& ) = delete;
		ClientLoginSession operator=( ClientLoginSession&& ) = delete;

		Connection_T connection;
	};
}
