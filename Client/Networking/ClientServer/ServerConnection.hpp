#pragma once

#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/Types.hpp"

#include "ClientServerCommon/Networking/ClientServer/Adapter.hpp"
#include "ClientServerCommon/Networking/ClientServer/MessageFactory.hpp"

#include "Common/Utility/Signal.hpp"
#include "Common/Utility/Timestep.hpp"

namespace Networking::ClientServer
{
	enum class ChannelType : YojimboPlugin::ChannelIndex_T;

	//
	// A client-side connection to a game server
	//
	class ServerConnection final
		: private yojimbo::Client
	{
		friend struct IServerConnectionOwner;

	public:
		using MessageHandlerCallback_T = std::function<bool( ServerConnection&, const yojimbo::Message& )>;
		using FactoryType = MessageFactory;

	public:
		ServerConnection( MessageHandlerCallback_T new_handler );
		~ServerConnection();

		using yojimbo::Client::IsConnected;
		using yojimbo::Client::IsConnecting;
		using yojimbo::Client::IsDisconnected;
		using yojimbo::Client::GetClientState;
		
		bool CanSendMessage( ChannelType type ) const;
		MessageFactory& GetMessageFactory();

		template<class MESSAGE_T>
		void SendMessage( ChannelType channel, const std::function<void(MESSAGE_T&)>& initialiser )
		{
			ASSERT( initialiser );
			if (!CanSendMessage( channel ))
			{
				ASSERT( false, "Cannot send message to given channel" );
				return;
			}

			if (auto* message = GetMessageFactory().CreateMessage<MESSAGE_T>())
			{
				initialiser( *message );
				::yojimbo::Client::SendMessage( static_cast<int>(channel), message );
			}

			FATAL( "Failed to allocate message of type" + MessageFactory::GetMessageName<MESSAGE_T>() );
		}

		void OnFixedUpdate( const PreciseTimestep& ts );

		void Connect( const yojimbo::Address& address );
		void Disconnect();


	public: // Signals
		Signal::signal<ServerConnection&> ConnectionStateChanged;


	private:
		void ProcessMessages();


	private:
		ClientServerAdapter adapter;
		const YojimboPlugin::ClientId_T client_id;

		MessageHandlerCallback_T message_handler_func;

		yojimbo::ClientState previous_client_state = yojimbo::ClientState::CLIENT_STATE_DISCONNECTED;
	};
}
