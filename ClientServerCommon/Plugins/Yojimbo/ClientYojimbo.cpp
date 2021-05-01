#include "ClientYojimbo.hpp"

#include "Common/Networking/Message.hpp"
#include "Common/Networking/MessageFactory.hpp"

namespace
{
	static const std::unordered_map<yojimbo::ClientState, Networking::Client::ConnectionState> ConnectionStateMapping = {
		{ yojimbo::ClientState::CLIENT_STATE_CONNECTED, Networking::Client::ConnectionState::Connected },
		{ yojimbo::ClientState::CLIENT_STATE_CONNECTING, Networking::Client::ConnectionState::Connecting },
		{ yojimbo::ClientState::CLIENT_STATE_DISCONNECTED, Networking::Client::ConnectionState::Disconnected },
		{ yojimbo::ClientState::CLIENT_STATE_ERROR, Networking::Client::ConnectionState::Disconnected },
	};
}

namespace Plugins::Network::Yojimbo
{
	ClientYojimbo::ClientYojimbo( 
		yojimbo::Address&& address_to_connect_to_
		, const std::array<uint8_t, yojimbo::KeyBytes>& private_key_
		, yojimbo::ClientServerConfig&& config_
		, ClientAdapter&& adapter_
	)
		: adapter( std::move( adapter_ ) )
		, config( std::move( config_ ) )
		, server_connection( yojimbo::GetDefaultAllocator(), yojimbo::Address( "0.0.0.0" ), config, adapter, 0.0 )
	{
		char address_string[256];
		address_to_connect_to_.ToString( address_string, sizeof( address_string ) );

		uint64_t client_id = 0;
		yojimbo::random_bytes( (uint8_t*)&client_id, 8 );
		LOG_INFO( Client, "Attempting connection to '{}' with client id {}", address_string, client_id );
		server_connection.InsecureConnect( private_key_.data(), client_id, address_to_connect_to_ );
	}

	ClientYojimbo::~ClientYojimbo()
	{
		if (!server_connection.IsDisconnected())
			server_connection.Disconnect();

		ASSERT( !server_connection.IsConnected() );
	}

	::Networking::Client::ConnectionState ClientYojimbo::GetState() const noexcept
	{
		return ConnectionStateMapping.at( server_connection.GetClientState() );
	}

	void ClientYojimbo::Disconnect()
	{
		wants_to_disconnect = true;
	}

	void ClientYojimbo::Update( const PreciseTimestep& timestep )
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

	void ClientYojimbo::ProcessMessages()
	{
		for (int channel = 0; channel < config.numChannels; ++channel)
		{
			auto* yojimbo_msg = server_connection.ReceiveMessage( channel );
			while (yojimbo_msg != nullptr)
			{
				if (message_handler_func)
				{
					auto message = adapter.GetFactory().CreateUntypedMessage( static_cast<::Networking::MessageType>(yojimbo_msg->GetType()) );
					// TODO: feed data into the message
					if (message)
						message_handler_func( *message );
				}

				// move to the next message
				server_connection.ReleaseMessage( yojimbo_msg );
				yojimbo_msg = server_connection.ReceiveMessage( channel );
			}
		}
	}
}
