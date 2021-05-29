#pragma once

#include <optional>
#include <ranges>
#include <variant>

#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/Types.hpp"
#include "ClientServerCommon/Networking/ClientServer/Adapter.hpp"

#include "Common/Utility/Signal.hpp"
#include "Common/Utility/Timestep.hpp"

namespace Networking::ClientServer
{
	class ActiveClient;
	class BaseClientConnection;
	class UnauthenticatedClient;
}

class GameServer final
{
	using ActiveClientPtr = std::unique_ptr<Networking::ClientServer::ActiveClient>;
	using UnauthenticatedClientPtr = std::unique_ptr<Networking::ClientServer::UnauthenticatedClient>;

	struct ClientEntry
	{
		bool to_be_disconnected = false;
		const time_t connected_timestamp;
		std::variant< UnauthenticatedClientPtr, ActiveClientPtr > client;

		bool IsActive() const noexcept;
		bool IsValid() const noexcept;

		Networking::ClientServer::ActiveClient* GetActive() const noexcept;
		Networking::ClientServer::UnauthenticatedClient* GetUnauthed() const noexcept;

		ClientEntry( const time_t connected_timestamp, std::variant< UnauthenticatedClientPtr, ActiveClientPtr > client );
	};
	using ClientContainer_T = std::unordered_map<YojimboPlugin::ClientIndex_T, ClientEntry>;

public:
	GameServer( yojimbo::Address host_address, const int max_clients );
	~GameServer();

	bool IsRunning() const noexcept;
	size_t GetConnectedClientCount() const noexcept;
	size_t GetMaxClientCount() const noexcept;

	// Mark a client connection as authenticated
	void AcceptClient( Networking::ClientServer::UnauthenticatedClient& client );
	void RejectClient( Networking::ClientServer::UnauthenticatedClient& client );

	void DisconnectClient( Networking::ClientServer::ActiveClient& client, std::optional<std::string> reason = std::nullopt );
	void DisconnectAllClients( std::optional<std::string> reason = std::nullopt );

	void OnFixedUpdate( const PreciseTimestep& ts );

	void ForEachClient( std::invocable<Networking::ClientServer::ActiveClient&> auto func, std::predicate<const Networking::ClientServer::ActiveClient&> auto predicate );
	void ForEachClient( std::invocable<const Networking::ClientServer::ActiveClient&> auto func, std::predicate<const Networking::ClientServer::ActiveClient&> auto predicate ) const;


	// Signals
	Signal::signal<Networking::ClientServer::ActiveClient&> ClientAccepted; // fired when a client is now active for play
	Signal::signal<Networking::ClientServer::ActiveClient&> ClientDisconnected; // fired when a previously accepted client is being disconnected

private:
	void ClientConnectedHandler( Networking::ClientServer::ClientServerAdapter& adapter, YojimboPlugin::ClientIndex_T index );
	void ClientDisconnectedHandler( Networking::ClientServer::ClientServerAdapter& adapter, YojimboPlugin::ClientIndex_T index );

	std::pair<YojimboPlugin::ClientIndex_T, ClientEntry*> FindClientEntry( const Networking::ClientServer::BaseClientConnection& client );
	std::pair<YojimboPlugin::ClientIndex_T, const ClientEntry*> FindClientEntry( const Networking::ClientServer::BaseClientConnection& client ) const;

	void DisconnectClient( ClientEntry& entry, std::optional<std::string> reason = std::nullopt );
	void ProcessMessages();

private:
	GameServer( const GameServer& ) = delete;
	GameServer( GameServer&& ) = delete;
	GameServer& operator=( const GameServer& ) = delete;
	GameServer& operator=( GameServer&& ) = delete;

private:
	Networking::ClientServer::ClientServerAdapter adapter;
	::yojimbo::Server server;
	ClientContainer_T clients;
};
