#include "ClientServerSession.hpp"

#include "ClientServerCommon/Networking/MessageId.hpp"
#include "ClientServerCommon/Networking/Messages.hpp"
#include "ClientServerCommon/Networking/MessageFactory.hpp"

namespace Sessions
{
	yojimbo::MessageFactory* ClientServerSession::ConnectionAdapter::CreateMessageFactory( yojimbo::Allocator& allocator )
	{
		return YOJIMBO_NEW( allocator, Networking::ClientServerGameMessageFactory, allocator );
		// TODO: do I need a destructor for this??
	}


	ClientServerSession::ClientServerSession( const yojimbo::Address& address )
		: adaptor()
		, connection_config()
		, server_connection( yojimbo::GetDefaultAllocator(), address, connection_config, adaptor, 0.0 )
	{
	}

	ClientServerSession::~ClientServerSession()
	{
	}

	ClientServerSession::ConnectionState ClientServerSession::GetConnectionState() const
	{
		if (server_connection.IsConnected())
			return ConnectionState::Connected;
		else if (server_connection.IsConnecting())
			return ConnectionState::Connecting;
		
		ASSERT( server_connection.IsDisconnected() );
		return ConnectionState::Disconnected;
	}

	void ClientServerSession::Disconnect()
	{
		disconnect_asap = true;
	}

	void ClientServerSession::Update( const PreciseTimestep& timestep )
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

		if (disconnect_asap && !server_connection.IsDisconnected())
			server_connection.Disconnect();
	}

	void ClientServerSession::ProcessMessages()
	{
		for (int channel = 0; channel < connection_config.numChannels; ++channel)
		{
			auto* message = server_connection.ReceiveMessage( channel );
			while (message != nullptr)
			{
				ProcessMessage( message );
				server_connection.ReleaseMessage( message );
				message = server_connection.ReceiveMessage( channel );
			}
		}
	}

	void ClientServerSession::ProcessMessage( yojimbo::Message* message )
	{
		switch (message->GetType())
		{
		case Networking::MessageId::ChatMessageCBId:
		{
			const auto* p = static_cast<Networking::Message::ChatMessageCB*>(message);
			std::string sender = p->sender;
			std::string content = p->message;

			LOG_INFO( Client, "Chat message received from '{}': {}", sender, content );
			// TODO: emit event
		}
		break;
		}
	}
}
