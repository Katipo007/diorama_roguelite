#include "ClientConnection.hpp"

#include "Server.hpp"

namespace YojimboPlugin
{
	ClientConnection::ClientConnection( BaseServer& owner_, const ClientId_T id_ )
		: owner( owner_ )
		, id( id_ )
		, connected_at( time( NULL ) )
	{
	}

	bool ClientConnection::IsDisconnecting() const
	{
		return owner.IsDisconnecting( *this );
	}

	void ClientConnection::Disconnect( std::optional<std::string> reason )
	{
		owner.DisconnectClient( *this, reason );
	}

	void ClientConnection::SendMessage( MessageType type, ChannelType channel, const MessageInitialiserFunc_T& initialiser )
	{
		owner.SendMessage( type, channel, *this, initialiser );
	}
}
