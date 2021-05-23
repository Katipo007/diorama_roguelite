#include "ServerConnection.hpp"

#include "ClientServerCommon/Networking/ClientServer/Config.hpp"
#include "Common/Utility/MagicEnum.hpp"

namespace
{
	YojimboPlugin::ClientId_T GenerateRandomClientId()
	{
		YojimboPlugin::ClientId_T client_id{ 0 };
		yojimbo::random_bytes( (uint8_t*)&client_id, sizeof( client_id ) );

		return client_id;
	}
}

namespace Networking::ClientServer
{
	ServerConnection::ServerConnection( const yojimbo::Address& target_address, MessageHandlerCallback_T message_handler_callback )
		: yojimbo::Client( yojimbo::GetDefaultAllocator(), yojimbo::Address( "0.0.0.0" ), MakeConfiguration(), adapter, 0.0 )
		, adapter()
		, client_id( GenerateRandomClientId() ) // If we ever create an accounts system or use Steam we'd want to use a unique fixed id per player.
		, message_handler_func( message_handler_callback )
	{
		if (!target_address.IsValid())
			throw std::runtime_error( "Invalid target address" );

		char address_string[256];
		target_address.ToString( address_string, static_cast<int>(std::size( address_string )) );

#if _DEBUG
		LOG_TRACE( Client, "Client id: {}", client_id );
#endif
		LOG_INFO( Client, "Attempting to connect to a game server at '{}'", address_string );

		// Connect
		// TODO: look into using the secure version of connect
		::yojimbo::Client::InsecureConnect( DefaultPrivateKey.data(), client_id, target_address );
	}

	ServerConnection::~ServerConnection()
	{
		Disconnect( true );

		ASSERT( yojimbo::Client::IsDisconnected() );
	}

	bool ServerConnection::CanSendMessage( ChannelType type ) const
	{
		return yojimbo::Client::CanSendMessage( static_cast<int>( type ) );
	}

	MessageFactory& ServerConnection::GetMessageFactory()
	{
		return static_cast<MessageFactory&>(::yojimbo::Client::GetMessageFactory());
	}

	void ServerConnection::OnFixedUpdate( const PreciseTimestep& ts )
	{
		ASSERT( ts.delta > 0 );

		if (wants_to_disconnect && !IsDisconnected())
			Disconnect( true );

		::yojimbo::Client::SendPackets();
		::yojimbo::Client::ReceivePackets();

		if (::yojimbo::Client::IsConnected())
			ProcessMessages();

		::yojimbo::Client::AdvanceTime( ::yojimbo::Client::GetTime() + ts.delta );

		if (wants_to_disconnect && !IsDisconnected())
			Disconnect( true );

		// Send connection state changed signal
		if (GetClientState() != previous_client_state)
		{
			ConnectionStateChanged( *this );
			previous_client_state = GetClientState();
		}
	}

	void ServerConnection::Disconnect( const bool immediately )
	{
		if (!immediately)
			wants_to_disconnect = true;
		else
		{
			if (!yojimbo::Client::IsDisconnected())
				yojimbo::Client::Disconnect();
		}
	}


	void ServerConnection::SetMessageHandler( MessageHandlerCallback_T new_handler )
	{
		message_handler_func = new_handler;
	}

	void ServerConnection::ProcessMessages()
	{
		for (ChannelType channel : magic_enum::enum_values<ChannelType>())
		{
			const int channel_index = static_cast<int>(channel);

			auto* message = ::yojimbo::Client::ReceiveMessage( channel_index );
			while (message != NULL)
			{
				const YojimboPlugin::MessageType_T message_type = message->GetType();
				const bool handled = !!message_handler_func && message_handler_func( *this, *message );

				::yojimbo::Client::ReleaseMessage( message );

				if (!handled)
				{
					Disconnect( true );
					LOG_CRITICAL( Client, "Disconnecting due to unhandled message. Message='{}'({})", MessageFactory::GetMessageName( message_type ), message_type );
					break;
				}

				message = ::yojimbo::Client::ReceiveMessage( channel_index );
			}
		}
	}
}
