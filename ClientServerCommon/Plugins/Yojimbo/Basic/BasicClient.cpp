#include "BasicClient.hpp"

#include "../MessageFactory.hpp"
#include "../Message.hpp"
#include "../YojimboPlugin.hpp"

namespace YojimboPlugin
{
	namespace
	{
		static const std::unordered_map<yojimbo::ClientState, BaseClient::ConnectionState> ConnectionStateMapping = {
			{ yojimbo::ClientState::CLIENT_STATE_CONNECTED, BaseClient::ConnectionState::Connected },
			{ yojimbo::ClientState::CLIENT_STATE_CONNECTING, BaseClient::ConnectionState::Connecting },
			{ yojimbo::ClientState::CLIENT_STATE_DISCONNECTED, BaseClient::ConnectionState::Disconnected },
			{ yojimbo::ClientState::CLIENT_STATE_ERROR, BaseClient::ConnectionState::Disconnected },
		};
	}


	BasicClient::BasicClient( Definition&& definition_ )
		: definition( std::move( definition_ ) )
		, server_connection( yojimbo::GetDefaultAllocator(), yojimbo::Address( definition.binding_address.data() ), definition.config, definition.adapter, 0.0 )
	{
		char address_string[256];
		yojimbo::Address target_address( definition.target_address.data() );
		ASSERT( target_address.IsValid() );
		target_address.ToString( address_string, sizeof( address_string ) );

		if (definition.plugin)
			definition.plugin->Add( *this );

		uint64_t client_id = 0;
		yojimbo::random_bytes( (uint8_t*)&client_id, 8 );
		LOG_INFO( Client, "Attempting connection to '{}' with client id {}", address_string, client_id );
		server_connection.InsecureConnect( definition.private_key.data(), client_id, target_address );
	}

	BasicClient::~BasicClient()
	{
		if (!server_connection.IsDisconnected())
			server_connection.Disconnect();

		ASSERT( !server_connection.IsConnected() );

		if (definition.plugin)
			definition.plugin->Remove( *this );
	}

	BaseClient::ConnectionState BasicClient::GetState() const noexcept
	{
		return ConnectionStateMapping.at( server_connection.GetClientState() );
	}

	void BasicClient::SendMessage( const MessageType type, const ChannelType channel, const std::function<void( Message& )>& initialiser )
	{
		ASSERT( initialiser );
		ASSERT( server_connection.CanSendMessage( static_cast<int>(channel) ) );
		auto& factory = definition.adapter.GetFactory();
		
		auto* message = factory.CreateUntypedMessage( type );
		ASSERT( !!message );
		if (!message)
			FATAL( "Failed to allocate message of type '" + std::string{ factory.GetMessageName( type ) } + "'" );

		
		initialiser( *message );
		server_connection.SendMessage( static_cast<int>(channel), message );
	}

	void BasicClient::Disconnect()
	{
		wants_to_disconnect = true;
	}

	void BasicClient::Update( const PreciseTimestep& timestep )
	{
		server_connection.SendPackets();

		server_connection.ReceivePackets();

		if (server_connection.IsConnected())
		{
			ProcessMessages();

			// ... do connected stuff ...
		}

		// update client connection
		server_connection.AdvanceTime( server_connection.GetTime() + timestep.delta );

		if (wants_to_disconnect && !server_connection.IsDisconnected())
			server_connection.Disconnect();

		// call connection state event
		if (server_connection.GetClientState() != previous_connection_state)
		{
			ConnectionStateChanged( *this );
			previous_connection_state = server_connection.GetClientState();
		}
	}

	void BasicClient::ProcessMessages()
	{
		for (int channel = 0; channel < definition.config.numChannels; ++channel)
		{
			auto* yojimbo_msg = server_connection.ReceiveMessage( channel );
			while (yojimbo_msg != nullptr)
			{
				try
				{
					if (message_handler)
						message_handler( *this, dynamic_cast<Message&>(*yojimbo_msg) );
				}
				catch (std::bad_cast& e)
				{
					LOG_CRITICAL( Client, "Failed to parse message: '{}'", e.what() );
					server_connection.Disconnect();
				}

				// move to the next message
				server_connection.ReleaseMessage( yojimbo_msg );
				yojimbo_msg = server_connection.ReceiveMessage( channel );
			}
		}
	}
}
