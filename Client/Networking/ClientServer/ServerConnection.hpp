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
	public:
		using MessageHandlerCallback_T = std::function<bool( ServerConnection&, const yojimbo::Message& )>;

	public:
		ServerConnection( const yojimbo::Address& target_address, MessageHandlerCallback_T message_handler_callback );
		~ServerConnection();

	public: // State
		using yojimbo::Client::IsConnected;
		using yojimbo::Client::IsConnecting;
		using yojimbo::Client::IsDisconnected;
		using yojimbo::Client::GetClientState;
		
		bool CanSendMessage( ChannelType type ) const;

	public: // Messaging
		template<class MESSAGE_T>
		void SendMessage( ChannelType channel, const std::function<void(MESSAGE_T&)>& initialiser )
		{
			ASSERT( initialiser );
			if (!CanSendMessage( channel ))
			{
				ASSERT( false, "Cannot send message to given channel" );
				return;
			}

			if (auto* message = GetFactory().CreateMessage<MESSAGE_T>())
			{
				initialiser( *message );
				::yojimbo::Client::SendMessage( static_cast<int>(channel), message );
			}

			FATAL( "Failed to allocate message of type" + MessageFactory::GetMessageName<MESSAGE_T>() );
		}


	public: // Getters
		MessageFactory& GetFactory() noexcept;


	public: // Methods
		void OnFixedUpdate( const PreciseTimestep& ts );

		void Disconnect( const bool immediately = false );

	public: // Signals

		Signal::signal<ServerConnection&> ConnectionStateChanged;

	private:
		void ProcessMessages();

	private:
		ClientServerAdapter adapter;
		const YojimboPlugin::ClientId_T client_id;

		bool wants_to_disconnect = false;
		MessageHandlerCallback_T message_handler_func;

		yojimbo::ClientState previous_client_state = yojimbo::ClientState::CLIENT_STATE_DISCONNECTED;
	};
}
