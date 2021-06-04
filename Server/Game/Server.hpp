#pragma once

#include <optional>
#include <variant>

#include "Server/Game/Networking/ActiveClientState.hpp"
#include "Server/Game/Networking/ClientHandle.hpp"
#include "Server/Game/Networking/LoginClientState.hpp"
#include "ClientServerCommon/ecs.hpp"
#include "ClientServerCommon/Game/Networking/Adapter.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"

#include "Common/Utility/Signal.hpp"
#include "Common/Utility/Timestep.hpp"

class Core;

namespace Game
{
	class Server final
	{
		using LoginClientsContainer_T = std::unordered_map<Networking::ClientIndex, Networking::LoginClientState>;
		using ActiveClientsContainer_T = std::unordered_map<Networking::ClientIndex, Networking::ActiveClientState>;

	public:
		Server( Core& core );
		~Server();

		ecs::Registry& GetRegistry() noexcept { return registry; }
		::yojimbo::Server& GetServer() noexcept { return server; }
		Networking::ClientHandle GetClientByIndex( Networking::ClientIndex client_index );
		Networking::ClientHandle GetClientById( const Networking::ClientId& client_id );

		void OnFixedUpdate( const PreciseTimestep& ts );
		void OnVariableUpdate( const PreciseTimestep& ts );

	private:
		void ClientConnectedHandler( Networking::Adapter& adapter, Networking::ClientIndex index );
		void ClientDisconnectedHandler( Networking::Adapter& adapter, Networking::ClientIndex index );

		void ProcessMessages();
		bool HandleMessage( Networking::ClientIndex index, const yojimbo::Message& message );
		void TickSimulation( const PreciseTimestep& ts );

	private:
		Server( const Server& ) = delete;
		Server( Server&& ) = delete;
		Server& operator=( const Server& ) = delete;
		Server& operator=( Server&& ) = delete;

	private:
		Core& core;
		Networking::Adapter network_adapter;
		::yojimbo::Server server;
		ecs::Registry registry;

		std::unordered_map<Networking::ClientId, Networking::ClientIndex> client_id_index_mapping;
		LoginClientsContainer_T unauthed_clients;
		ActiveClientsContainer_T active_clients;
	};
}
