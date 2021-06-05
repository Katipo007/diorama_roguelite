#include "ClientServerSession.hpp"

#include "ClientServerCommon/Game/Networking/Config.hpp"
#include "Client/Game/Systems/ClientSystem.hpp"

#include "Common/Utility/MagicEnum.hpp"

namespace Game::Networking
{
	ClientServerSession::ClientServerSession( ClientId client_id, const yojimbo::Address& destination )
		: network_adapter()
		, client( yojimbo::GetDefaultAllocator(), yojimbo::Address( "0.0.0.0" ), MakeConfiguration(), network_adapter, 0.0 )
	{
		char address_string[256];
		destination.ToString( address_string, static_cast<int>(std::size( address_string )) );

#if _DEBUG
		LOG_TRACE( LoggingChannels::Client, "Client id: {}", client_id );
#endif
		LOG_INFO( LoggingChannels::Client, "Attempting to connect to a game server at '{}'", address_string );

		client.InsecureConnect( DefaultPrivateKey.data(), client_id, destination );
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
