#include "BaseClientSession.hpp"

#include <algorithm>
#include <functional>

#include "Client/Networking/ClientServer/ServerConnection.hpp"

namespace Sessions
{
	BaseClientSession::BaseClientSession( std::unique_ptr<Networking::ClientServer::ServerConnection>& connection_ )
		: connection( std::move( connection_ ) )
	{
		if (!!connection)
			connection->SetMessageHandler( std::bind( &BaseClientSession::MessageHandler, this, std::placeholders::_1, std::placeholders::_2 ) );
	}

	BaseClientSession::~BaseClientSession()
	{
		if( !!connection )
			static_cast<void>(ReleaseConnection());
	}

	bool BaseClientSession::Expired() const noexcept
	{
		return !connection || connection->IsDisconnected();
	}

	std::unique_ptr<Networking::ClientServer::ServerConnection> BaseClientSession::ReleaseConnection()
	{
		if (!connection)
			return nullptr;

		connection->ResetMessageHandler();
		return std::move( connection );
	}

	void BaseClientSession::OnFixedUpdate( const PreciseTimestep& ts )
	{
		if (!!connection)
			connection->OnFixedUpdate( ts );
	}

	bool BaseClientSession::MessageHandler( Networking::ClientServer::ServerConnection&, const yojimbo::Message& )
	{
		return false;
	}

	Networking::ClientServer::ServerConnection* BaseClientSession::GetConnection()
	{
		return connection.get();
	}

	const Networking::ClientServer::ServerConnection* BaseClientSession::GetConnection() const
	{
		return connection.get();
	}
}
