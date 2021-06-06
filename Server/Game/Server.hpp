#pragma once

#include <optional>
#include <variant>

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
	public:
		Server( Core& core, yojimbo::Address host_address );
		~Server();

		ecs::Registry& GetRegistry() noexcept { return registry; }
		::yojimbo::Server& GetServer() noexcept { return server; }
		ecs::EntityHandle GetClientByIndex( Networking::ClientIndex client_index );
		ecs::EntityHandle GetClientById( const Networking::ClientId& client_id );

		void OnFixedUpdate( const PreciseTimestep& ts );
		void OnVariableUpdate( const PreciseTimestep& ts );

	private:
		void ClientConnectedHandler( Networking::Adapter& adapter, Networking::ClientIndex index );
		void ClientDisconnectedHandler( Networking::Adapter& adapter, Networking::ClientIndex index );

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
		std::unordered_map<Networking::ClientIndex, ecs::EntityHandle> client_entities;
	};
}
