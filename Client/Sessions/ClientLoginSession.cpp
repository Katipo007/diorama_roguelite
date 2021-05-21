#include "ClientLoginSession.hpp"

#include <algorithm>
#include <functional>

#include "Client/Networking/ClientServer/ServerConnection.hpp"

namespace Sessions
{
	ClientLoginSession::ClientLoginSession( Connection_T&& connection_ )
		: connection( std::move( connection_ ) )
	{
		if (!!connection)
			connection->SetMessageHandler( std::bind( &ClientLoginSession::MessageHandler, this, std::placeholders::_1, std::placeholders::_2 ) );
	}

	ClientLoginSession::~ClientLoginSession()
	{
		static_cast<void>(ReleaseConnection());
	}

	bool ClientLoginSession::Expired() const
	{
		return !connection || connection->IsDisconnected();
	}

	ClientLoginSession::Connection_T ClientLoginSession::ReleaseConnection()
	{
		if (!connection)
			return nullptr;

		connection->ResetMessageHandler();
		return std::move( connection );
	}

	bool ClientLoginSession::MessageHandler( Networking::ClientServer::ServerConnection&, const yojimbo::Message& msg )
	{
		switch (msg.GetType())
		{
		default:
			return false;
		}
	}
}
