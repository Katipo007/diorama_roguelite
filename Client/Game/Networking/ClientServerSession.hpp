#pragma once

#include <concepts>

#include "ClientServerCommon/Game/Networking.hpp"
#include "ClientServerCommon/Game/Networking/Adapter.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"
#include "ClientServerCommon/ecs.hpp"

#include "Common/Utility/Signal.hpp"
#include "Common/Utility/Timestep.hpp"

namespace Game::Networking
{
	//
	// A client-side connection to a game server
	//
	class ClientServerSession final
	{
	public:
		ClientServerSession( ClientId client_id, const yojimbo::Address& address );
		~ClientServerSession();

		void OnFixedUpdate( const PreciseTimestep& ts );
		void OnVariableUpdate( const PreciseTimestep& ts );

		bool IsConnected() const noexcept;
		bool IsConnecting() const noexcept;
		bool IsDisconnected() const noexcept;
		void Disconnect();

		Signal::signal<ClientServerSession&> ConnectionStateChanged;

	private:
		void TickSimulation( const PreciseTimestep& ts );

	private:
		Networking::Adapter network_adapter;
		yojimbo::Client client;
		ecs::Registry registry;
		yojimbo::ClientState previous_connection_state = yojimbo::ClientState::CLIENT_STATE_DISCONNECTED;
	};
}
