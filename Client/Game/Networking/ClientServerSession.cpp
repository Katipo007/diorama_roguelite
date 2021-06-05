#include "ClientServerSession.hpp"

#include "ClientServerCommon/Game/Networking/Config.hpp"
#include "ClientServerCommon/Game/Networking/MessageFactory.hpp"
#include "Client/Game/Systems/ClientSystem.hpp"

#include "Common/Utility/MagicEnum.hpp"
#include "Common/Utility/StringUtility.hpp"

namespace Game::Networking
{
	ClientServerSession::ClientServerSession( ServerConnectionRequest request_ )
		: connection_request{ std::move( request_ ) }
		, network_adapter{}
		, client{ yojimbo::GetDefaultAllocator(), yojimbo::Address( "0.0.0.0" ), MakeConfiguration(), network_adapter, 0.0 }
	{
		yojimbo::Address destination{ connection_request.destination.data() };
		if (!destination.IsValid())
			throw std::runtime_error{ "Invalid address" };

		std::array<char, 256> address_string{ 0 };
		destination.ToString( &address_string[0], static_cast<int>(std::size( address_string )) );

#if _DEBUG
		LOG_TRACE( LoggingChannels::Client, "Client id: {}", connection_request.client_id );
#endif
		LOG_INFO( LoggingChannels::Client, "Attempting to connect to a game server at '{}'", address_string.data() );

		client.InsecureConnect( DefaultPrivateKey.data(), connection_request.client_id, destination );
	}

	ClientServerSession::~ClientServerSession()
	{
		if (!client.IsDisconnected())
			client.Disconnect();
	}

	void ClientServerSession::OnFixedUpdate( const PreciseTimestep& ts )
	{
		Systems::ClientProcessIncoming( client, registry, ts );

		if (previous_connection_state != client.GetClientState())
		{
			ConnectionStateChanged( *this );
			previous_connection_state = client.GetClientState();

			// Send request info
			if (client.IsConnected())
			{
				auto* request = dynamic_cast<Messages::ClientServerLoginStart*>(client.CreateMessage( MessageFactory::GetMessageType<Messages::ClientServerLoginStart>() ));
				ASSERT( request != nullptr );
				StringUtility::StringToArray( connection_request.username, request->username );
				client.SendMessage( magic_enum::enum_integer( ChannelType::Reliable ), request );
			}
		}

		TickSimulation( ts );

		Systems::ClientProcessOutgoing( client, registry, ts );
	}

	void Game::Networking::ClientServerSession::OnVariableUpdate( const PreciseTimestep& )
	{

	}

	bool ClientServerSession::IsConnected() const noexcept
	{
		return client.IsConnected();
	}

	bool ClientServerSession::IsConnecting() const noexcept
	{
		return client.IsConnecting();
	}

	void Game::Networking::ClientServerSession::TickSimulation( const PreciseTimestep& ts )
	{
		(void)ts;
	}

	bool Game::Networking::ClientServerSession::IsDisconnected() const noexcept
	{
		return client.IsDisconnected();
	}

	void ClientServerSession::Disconnect()
	{
		if (!client.IsDisconnected())
			client.Disconnect();
	}
}
