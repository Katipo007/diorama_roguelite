#include "ClientServerSession.hpp"

#include "ClientServerCommon/Game/Networking/Config.hpp"
#include "ClientServerCommon/Game/Networking/MessageFactory.hpp"
#include "Client/Game/ClientSync/ClientSyncHelpers.hpp"
#include "ClientServerSessionEvents.hpp"

#include "Common/Utility/MagicEnum.hpp"
#include "Common/Utility/StringUtility.hpp"

namespace Events = Game::Networking::ClientServerSessionEvents;

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

	void ClientServerSession::SendChatMessage( std::string_view message )
	{
		SendMessage<Messages::ClientServerChatMessage>( ChannelType::Unreliable, [&]( Messages::ClientServerChatMessage& chat )
			{
				StringUtility::StringToArray( message, chat.message );
			} );
	}

	void ClientServerSession::OnFixedUpdate( const PreciseTimestep& ts )
	{
		if (client.IsConnected())
			client.ReceivePackets();

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

		ProcessMessages();
		dispatcher.update();
		TickSimulation( ts );

		if (client.IsConnected())
			client.SendPackets();

		client.AdvanceTime( client.GetTime() + ts.delta );
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

	void ClientServerSession::ProcessMessages()
	{
		if (!client.IsConnected())
			return;

		for (auto channel : magic_enum::enum_values<Game::Networking::ChannelType>())
		{
			const auto channel_idx = static_cast<YojimboPlugin::ChannelIndex_T>(channel);
			yojimbo::Message* message = client.ReceiveMessage( channel_idx );
			while (message != NULL)
			{
				const auto message_type = message->GetType();
				const bool handled = ProcessMessage( *message, channel );
				client.ReleaseMessage( message );

				if (!handled)
				{
					LOG_WARN( LoggingChannels::Client, "Disconnecting from server due to unhandled message of type '{}'({})", Game::Networking::MessageFactory::GetMessageName( message_type ), message_type );
					client.Disconnect();
					return;
				}

				message = client.ReceiveMessage( channel_idx );
			}
		}
	}

	bool ClientServerSession::ProcessMessage( const yojimbo::Message& message, const Networking::ChannelType channel )
	{
		//
		// Common messages
		//
		switch (message.GetType())
		{
			case MessageFactory::GetMessageType<Messages::ServerClientDisconnect>() :
			{
				const auto& disconnection = static_cast<const Messages::ServerClientDisconnect&>(message);
				disconnection_reason = std::string_view{ disconnection.reason.data() };
				client.Disconnect();
				return true;
			}
		}

		//
		// Login messages
		//
		if (!TestSessionFlag( Flags::IsAccepted ))
		{
			switch (message.GetType())
			{
				case MessageFactory::GetMessageType<Messages::ServerClientLoginSuccess>() :
				{
					const auto& result = static_cast<const Messages::ServerClientLoginSuccess&>(message);
					(void)result;
					SetSessionFlag( Flags::IsAccepted );
					return true;
				}
			}
		}
		//
		// Active messages
		//
		else
		{
			switch (message.GetType())
			{
				case MessageFactory::GetMessageType<Messages::ServerClientChatMessage>() :
				{
					const auto& chat = static_cast<const Messages::ServerClientChatMessage&>(message);
					dispatcher.trigger<Events::ChatMessageReceived>( chat.sender.data(), chat.message.data() );
					return true;
				}

				case MessageFactory::GetMessageType<ClientSync::Messages::ServerClientAddEntity>() :
					return ClientSync::Helpers::HandleMessage( registry, synced_entities, static_cast<const ClientSync::Messages::ServerClientAddEntity&>(message) );

				case MessageFactory::GetMessageType<ClientSync::Messages::ServerClientEntitySync>() :
					return ClientSync::Helpers::HandleMessage( synced_entities, static_cast<const ClientSync::Messages::ServerClientEntitySync&>(message), channel );

				case MessageFactory::GetMessageType<ClientSync::Messages::ServerClientRemoveEntity>() :
					return ClientSync::Helpers::HandleMessage( synced_entities, static_cast<const ClientSync::Messages::ServerClientRemoveEntity&>(message) );
			}
		}

		return false;
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
