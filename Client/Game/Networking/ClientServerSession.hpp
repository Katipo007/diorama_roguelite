#pragma once

#include "ClientServerCommon/Game/Networking.hpp"
#include "ClientServerCommon/Game/Networking/Adapter.hpp"
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

		void OnFixedUpdate( const PreciseTimestep& ts );
		void OnVariableUpdate( const PreciseTimestep& ts );

		bool IsConnected() const noexcept;
		bool IsConnecting() const noexcept;
		bool IsDisconnected() const noexcept;
		void Disconnect();
		const std::optional<std::string> GetDisconnectionReason() const noexcept { return disconnection_reason; }

		const ServerConnectionRequest& GetConnectionRequest() const noexcept { return connection_request; } // return the data that was used to initialise this session

		Signal::signal<ClientServerSession&> ConnectionStateChanged;

		bool TestSessionFlag( Flags flag ) const noexcept { return session_flags.test( magic_enum::enum_index( flag ).value() ); }
		const auto& GetSessionFlags() const noexcept { return session_flags; }

	private:
		void ProcessMessages();
		bool ProcessMessage( const yojimbo::Message& message );
		void TickSimulation( const PreciseTimestep& ts );

		void SetSessionFlag( Flags flag, const bool value = true ) noexcept { session_flags.set( magic_enum::enum_index( flag ).value(), value ); }
		void ClearSessionFlag( Flags flag ) noexcept { SetSessionFlag( flag, false ); }

	private:
		const ServerConnectionRequest connection_request;
		Networking::Adapter network_adapter;
		yojimbo::Client client;
		ecs::Registry registry;

		std::optional<std::string> disconnection_reason;

		std::bitset<magic_enum::enum_count<Flags>()> session_flags;
		std::string username;

		yojimbo::ClientState previous_connection_state = yojimbo::ClientState::CLIENT_STATE_DISCONNECTED;
	};
}
