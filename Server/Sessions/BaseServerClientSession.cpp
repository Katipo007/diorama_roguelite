#include "BaseServerClientSession.hpp"

#include "Server/Networking/ClientServer/ServerClientConnection.hpp"

namespace Sessions
{
	BaseServerClientSession::BaseServerClientSession( std::unique_ptr<Networking::ClientServer::ServerClientConnection> connection_ )
		: connection( std::move( connection_ ) )
	{
		ASSERT( !!connection );
		if (!!connection)
			connection->SetMessageHandler( std::bind( &BaseServerClientSession::InternalMessageHandler, this, std::placeholders::_1, std::placeholders::_2 ) );
	}

	Sessions::BaseServerClientSession::~BaseServerClientSession()
	{
		static_cast<void>(ReleaseConnection());
	}

	bool BaseServerClientSession::Expired() const noexcept
	{
		return !connection || connection->Expired();
	}

	std::unique_ptr<Networking::ClientServer::ServerClientConnection> BaseServerClientSession::ReleaseConnection()
	{
		if (!!connection)
		{
			connection->ResetMessageHandler();
			return std::move( connection );
		}

		return nullptr;
	}

	void BaseServerClientSession::OnFixedUpdate( const PreciseTimestep& ts )
	{
		if (!!connection)
			connection->OnFixedUpdate( ts );
	}

	Networking::ClientServer::ServerClientConnection* BaseServerClientSession::GetConnection()
	{
		return connection.get();
	}

	const Networking::ClientServer::ServerClientConnection* BaseServerClientSession::GetConnection() const
	{
		return connection.get();
	}

	bool BaseServerClientSession::InternalMessageHandler( Networking::ClientServer::ServerClientConnection& from, const yojimbo::Message& message )
	{
		if (!connection || (&from == connection.get()))
			return false;

		return HandleMessage( message );
	}
}
