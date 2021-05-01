#include "ClientConnection.hpp"

#include "Server.hpp"
#include "Message.hpp"

namespace Networking
{
	ClientConnection::ClientConnection( Server& owner_ )
		: owner( owner_ )
		, connected_at( time( NULL ) )
	{
	}

	ClientId_T ClientConnection::GetId() const noexcept
	{
		return owner.GetClientId( *this );
	}

	void ClientConnection::SendMessage( std::unique_ptr<Message> message, ChannelType channel )
	{
		owner.SendMessage( std::move( message ), channel, *this );
	}

	bool ClientConnection::ProcessMessage( const Message& message )
	{
		(void)message;
		NOT_IMPLEMENTED; // TODO
		return false;
	}
}
