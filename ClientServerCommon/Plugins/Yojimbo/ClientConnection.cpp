#include "ClientConnection.hpp"

#include "Server.hpp"

namespace YojimboPlugin
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
