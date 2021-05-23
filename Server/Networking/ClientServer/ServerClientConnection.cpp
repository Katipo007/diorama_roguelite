#include "ServerClientConnection.hpp"

#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"
#include "ClientServerCommon/Networking/ClientServer/Config.hpp"
#include "ClientServerCommon/Networking/ClientServer/MessageFactory.hpp"

#include "Server/GameServer.hpp"

#include "Common/Utility/MagicEnum.hpp"
#include "Common/Utility/StringUtility.hpp"

namespace Networking::ClientServer
{
	ServerClientConnection::ServerClientConnection( yojimbo::Server& server_, YojimboPlugin::ClientIndex_T index_ ) noexcept
		: server( server_ )
		, client_index( index_ )
		, connected_at_timestamp( time( NULL ) )
	{
	}

	ServerClientConnection::~ServerClientConnection() noexcept
	{
		Disconnect();
	}

	bool ServerClientConnection::IsConnected() const noexcept
	{
		return server.IsClientConnected( client_index );
	}

	YojimboPlugin::ClientId_T ServerClientConnection::GetId() const noexcept
	{
		return server.GetClientId( client_index );
	}

	void ServerClientConnection::OnFixedUpdate( const PreciseTimestep& ts )
	{
		(void)ts;
		ProcessMessages();
	}

	void ServerClientConnection::Disconnect( std::optional<std::string> reason )
	{
		if (being_destroyed_from_disconnect_handler || !server.IsClientConnected( client_index ))
			return;

		if (reason.has_value())
		{
			auto* message = dynamic_cast<Messages::ServerClientDisconnect*>(server.CreateMessage( client_index, MessageFactory::GetMessageType<Messages::ServerClientDisconnect>() ));
			StringUtility::StringToArray( *reason, message->reason );
			SendMessageInternal( ChannelType::Unreliable, message );
		}

		server.DisconnectClient( client_index );
	}

	void ServerClientConnection::SetMessageHandler( MessageHandlerCallback_T callback )
	{
		message_handler = callback;
	}

	void ServerClientConnection::ProcessMessages()
	{
		ASSERT( server.IsRunning() );
		if (!IsConnected())
			return;

		yojimbo::Message* message = nullptr;
		for (ChannelType channel : magic_enum::enum_values<ChannelType>())
		{
			message = server.ReceiveMessage( client_index, static_cast<int>(channel) );
			while (message != nullptr)
			{
				const bool handled = !!message_handler && message_handler( *this, *message );
				server.ReleaseMessage( client_index, message );

				// disconnect if message went unhandled
				if (!handled)
				{
					LOG_INFO( Server, "Disconnecting client due to unhandled message" );
					Disconnect();
					return;
				}

				// move onto the next message
				message = server.ReceiveMessage( client_index, static_cast<int>(channel) );
			}
		}
	}

	bool ServerClientConnection::SendMessageInternal( ChannelType channel, yojimbo::Message* message )
	{
		if (!CanSendMessage( channel ))
			return false;

		server.SendMessage( client_index, static_cast<int>(channel), message );
		return true;
	}

	void ServerClientConnection::FlushMessages()
	{
		// TODO
	}

	bool ServerClientConnection::CanSendMessage( ChannelType channel ) const noexcept
	{
		return server.CanSendMessage( client_index, static_cast<int>( channel ) );
	}
	
	bool ServerClientConnection::IsLoopbackClient() const noexcept
	{
		return server.IsLoopbackClient( client_index );
	}
}
