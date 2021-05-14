#include "BasicClient.hpp"

#include "../MessageFactory.hpp"
#include "../Message.hpp"

namespace YojimboPlugin
{
	namespace
	{
		static const std::unordered_map<yojimbo::ClientState, Client::ConnectionState> ConnectionStateMapping = {
			{ yojimbo::ClientState::CLIENT_STATE_CONNECTED, Client::ConnectionState::Connected },
			{ yojimbo::ClientState::CLIENT_STATE_CONNECTING, Client::ConnectionState::Connecting },
			{ yojimbo::ClientState::CLIENT_STATE_DISCONNECTED, Client::ConnectionState::Disconnected },
			{ yojimbo::ClientState::CLIENT_STATE_ERROR, Client::ConnectionState::Disconnected },
		};
	}


	BasicClient::BasicClient(
		const Address& address_to_connect_to_
		, const std::array<uint8_t, yojimbo::KeyBytes>& private_key_
		, yojimbo::ClientServerConfig&& config_
		, BasicAdapter&& adapter_
	)
		: config( std::move( config_ ) )
		, adapter( std::move( adapter_ ) )
		, server_connection( yojimbo::GetDefaultAllocator(), yojimbo::Address( "0.0.0.0" ), config, adapter, 0.0 )
	{
		char address_string[256];
		yojimbo::Address target_address( address_to_connect_to_.c_str() );
		ASSERT( target_address.IsValid() );
		target_address.ToString( address_string, sizeof( address_string ) );

		uint64_t client_id = 0;
		yojimbo::random_bytes( (uint8_t*)&client_id, 8 );
		LOG_INFO( Client, "Attempting connection to '{}' with client id {}", address_string, client_id );
		server_connection.InsecureConnect( private_key_.data(), client_id, target_address );
	}

	BasicClient::~BasicClient()
	{
		if (!server_connection.IsDisconnected())
			server_connection.Disconnect();

		ASSERT( !server_connection.IsConnected() );
	}

	Client::ConnectionState BasicClient::GetState() const noexcept
	{
		return ConnectionStateMapping.at( server_connection.GetClientState() );
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
		for (int channel = 0; channel < config.numChannels; ++channel)
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
