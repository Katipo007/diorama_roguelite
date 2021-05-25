#pragma once

#include <optional>
#include <variant>

#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/Types.hpp"
#include "ClientServerCommon/Networking/ClientServer/Adapter.hpp"

#include "Common/Utility/Timestep.hpp"

namespace Sessions
{
	class BaseServerClientSession;
}

class GameServer final
	: private yojimbo::Server
{
	struct ClientEntry
	{
		bool to_be_disconnected = false;
		std::unique_ptr<Sessions::BaseServerClientSession> session;

		bool Expired() const;
		ClientEntry( std::unique_ptr<Sessions::BaseServerClientSession> session );
	};
	using ClientContainer_T = std::unordered_map<YojimboPlugin::ClientIndex_T, ClientEntry>;

public:
	GameServer( yojimbo::Address host_address, const int max_clients );
	~GameServer();

	using yojimbo::Server::IsRunning;
	using yojimbo::Server::GetNumConnectedClients;
	using yojimbo::Server::GetMaxClients;

	using yojimbo::Server::DisconnectAllClients;

	void OnFixedUpdate( const PreciseTimestep& ts );

private:
	void ClientConnectedHandler( Networking::ClientServer::ClientServerAdapter& adapter, YojimboPlugin::ClientIndex_T index );
	void ClientDisconnectedHandler( Networking::ClientServer::ClientServerAdapter& adapter, YojimboPlugin::ClientIndex_T index );

private:
	GameServer( const GameServer& ) = delete;
	GameServer( GameServer&& ) = delete;
	GameServer& operator=( const GameServer& ) = delete;
	GameServer& operator=( GameServer&& ) = delete;

private:
	Networking::ClientServer::ClientServerAdapter adapter;
	ClientContainer_T clients;
};
