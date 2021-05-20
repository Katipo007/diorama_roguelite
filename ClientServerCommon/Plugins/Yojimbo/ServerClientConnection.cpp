#include "ServerClientConnection.hpp"

#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"

namespace YojimboPlugin
{
	ServerClientConnection::ServerClientConnection( yojimbo::BaseServer& server_, ClientIndex_T index_ )
		: server( server_ )
		, index( index_ )
	{
	}

	ServerClientConnection::~ServerClientConnection()
	{
		Disconnect();
	}

	bool ServerClientConnection::IsConnected() const
	{
		return server.IsClientConnected( index );
	}

	YojimboPlugin::ClientId_T ServerClientConnection::GetId() const noexcept
	{
		return server.GetClientId( index );
	}

	void ServerClientConnection::Disconnect()
	{
		if (IsConnected())
			server.DisconnectClient( index );
	}

	bool ServerClientConnection::SendMessage( ChannelIndex_T channel, yojimbo::Message* message )
	{
		if (!CanSendMessage( channel ))
			return false;

		server.SendMessage( index, static_cast<int>(channel), message );
		return true;
	}

	bool ServerClientConnection::CanSendMessage( ChannelIndex_T channel ) const
	{
		return server.CanSendMessage( index, static_cast<int>( channel ) );
	}

	void ServerClientConnection::ProcessMessages( ChannelIndex_T channel, const MessageHandlerCallback_T& handler )
	{
		ASSERT( server.IsRunning() );
		ASSERT( !!handler, "No handler provided" );
		if (!handler)
		{
			Disconnect();
			return;
		}

		if (!IsConnected())
			return;

		yojimbo::Message* message = nullptr;
		message = server.ReceiveMessage( index, channel );
		while (message != nullptr && IsConnected())
		{
			const bool handled = handler( *this, *message );
			server.ReleaseMessage( index, message );

			// disconnect if message went unhandled
			if (!handled)
			{
				LOG_INFO( Server, "Disconnecting client due to unhandled message" );
				Disconnect();
				return;
			}

			// move onto the next message
			message = server.ReceiveMessage( index, channel );
		}
	}

	bool ServerClientConnection::IsLoopbackClient() const
	{
		return server.IsLoopbackClient( index );
	}
}
