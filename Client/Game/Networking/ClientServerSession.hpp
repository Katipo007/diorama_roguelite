#pragma once

#include "ClientServerCommon/Game/Networking.hpp"
#include "ClientServerCommon/Game/Networking/Adapter.hpp"
#include "ClientServerCommon/Game/Networking/Channels.hpp"
#include "ClientServerCommon/Game/Networking/MessageFactory.hpp"
#include "ClientServerCommon/Game/ClientSync/ClientSyncTypes.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"
#include "ClientServerCommon/ecs.hpp"
#include "ServerConnectionRequest.hpp"

#include "Common/Utility/MagicEnum.hpp"
#include "Common/Utility/Signal.hpp"
#include "Common/Utility/Timestep.hpp"

#include <bitset>
#include <concepts>

namespace Game::Networking
{
	//
	// A client-side connection to a game server
	//
	class ClientServerSession final
	{
	public:
		enum class Flags
		{
			IsAccepted, //< Has this session been accepted by the server
		};

	public:
		ClientServerSession( ServerConnectionRequest request );
		~ClientServerSession();

		bool TestSessionFlag( Flags flag ) const noexcept { return session_flags.test( magic_enum::enum_index( flag ).value() ); }
		const auto& GetSessionFlags() const noexcept { return session_flags; }

		void SendChatMessage( std::string_view message );

		template<YojimboPlugin::Concepts::Message T>
		bool SendMessage( Networking::ChannelType channel, std::invocable<T&> auto initialiser )
		{
			const auto channel_idx = magic_enum::enum_integer( channel );

			if (!client.IsConnected() || !client.CanSendMessage( channel_idx ))
				return false;

			if (auto* message = static_cast<T*>(client.CreateMessage( Networking::MessageFactory::GetMessageType<T>() )))
			{
				initialiser( *message );
				client.SendMessage( channel_idx, message );
				return true;
			}

			FATAL( "Failed to allocate message" );
		}

		bool IsConnected() const noexcept;
		bool IsConnecting() const noexcept;
		bool IsDisconnected() const noexcept;
		void Disconnect();
		const std::optional<std::string> GetDisconnectionReason() const noexcept { return disconnection_reason; }
		const ServerConnectionRequest& GetConnectionRequest() const noexcept { return connection_request; } // return the data that was used to initialise this session

		void OnFixedUpdate( const PreciseTimestep& ts );
		void OnVariableUpdate( const PreciseTimestep& ts );

		Signal::signal<ClientServerSession&> ConnectionStateChanged;

		ecs::Registry& GetRegistry() noexcept { return registry; }
		const ecs::Registry& GetRegistry() const noexcept { return registry; }

		template<class Event>
		[[nodiscard]] decltype(auto) Sink() { return dispatcher.sink<Event>(); }

	private:
		void ProcessMessages();
		bool ProcessMessage( const yojimbo::Message& message, const Networking::ChannelType channel );
		void TickSimulation( const PreciseTimestep& ts );

		void SetSessionFlag( Flags flag, const bool value = true ) noexcept { session_flags.set( magic_enum::enum_index( flag ).value(), value ); }
		void ClearSessionFlag( Flags flag ) noexcept { SetSessionFlag( flag, false ); }

	private:
		const ServerConnectionRequest connection_request;
		Networking::Adapter network_adapter;
		yojimbo::Client client;
		ecs::Registry registry;
		entt::dispatcher dispatcher;

		ClientSync::SyncedEntityMap_T synced_entities;

		std::optional<std::string> disconnection_reason;

		std::bitset<magic_enum::enum_count<Flags>()> session_flags;

		yojimbo::ClientState previous_connection_state = yojimbo::ClientState::CLIENT_STATE_DISCONNECTED;
	};
}
