#pragma once

#include <concepts>
#include <optional>

#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/Types.hpp"
#include "ClientServerCommon/Networking/ClientServer/Adapter.hpp"
#include "ClientServerCommon/Networking/ClientServer/Config.hpp"
#include "ClientServerCommon/Networking/ClientServer/MessageFactory.hpp"

#include "Common/Utility/Signal.hpp"
#include "Common/Utility/Timestep.hpp"

namespace yojimbo { class Message; }

namespace Networking::ClientServer
{
	class ActiveClient;
	class BaseClientConnection;
	class UnauthenticatedClient;
}

class GameServer final
{
public:
	GameServer( yojimbo::Address host_address, const int max_clients );
	~GameServer();

	bool IsRunning() const noexcept;
	size_t GetConnectedClientCount() const noexcept;
	size_t GetMaxClientCount() const noexcept;

	// Mark a client connection as authenticated
	bool AcceptClient( Networking::ClientServer::UnauthenticatedClient& client );
	bool RejectClient( Networking::ClientServer::UnauthenticatedClient& client );

	void DisconnectClient( Networking::ClientServer::BaseClientConnection& client, std::optional<std::string> reason = std::nullopt );
	void DisconnectAllClients( std::optional<std::string> reason = std::nullopt );

	void OnFixedUpdate( const PreciseTimestep& ts );

	void ForEachClient( std::invocable<Networking::ClientServer::ActiveClient&> auto func, std::predicate<const Networking::ClientServer::ActiveClient&> auto predicate );
	void ForEachClient( std::invocable<const Networking::ClientServer::ActiveClient&> auto func, std::predicate<const Networking::ClientServer::ActiveClient&> auto predicate ) const;

	bool SendMessage( Networking::ClientServer::BaseClientConnection& client, Networking::ClientServer::ChannelType channel, YojimboPlugin::MessageType_T type, const std::function<void(yojimbo::Message&)>& initialiser );
	template<class T>
	bool SendMessage( Networking::ClientServer::BaseClientConnection& client, Networking::ClientServer::ChannelType channel, const std::function<void( yojimbo::Message& )>& initialiser )
	{
		return SendMessage( client, channel, Networking::ClientServer::MessageFactory::template GetMessageType<T>(), initialiser );
	}

	// Signals
	Signal::signal<Networking::ClientServer::ActiveClient&> ClientAccepted; // fired when a client is now active for play
	Signal::signal<Networking::ClientServer::ActiveClient&> ClientDisconnected; // fired when a previously accepted client is being disconnected

private:
	void ClientConnectedHandler( Networking::ClientServer::ClientServerAdapter& adapter, YojimboPlugin::ClientIndex_T index );
	void ClientDisconnectedHandler( Networking::ClientServer::ClientServerAdapter& adapter, YojimboPlugin::ClientIndex_T index );

	void ProcessMessages();

	void ForAllConnections( std::invocable<Networking::ClientServer::BaseClientConnection&> auto func );
	Networking::ClientServer::BaseClientConnection* FindClientConnection( YojimboPlugin::ClientIndex_T client_index );

private:
	GameServer( const GameServer& ) = delete;
	GameServer( GameServer&& ) = delete;
	GameServer& operator=( const GameServer& ) = delete;
	GameServer& operator=( GameServer&& ) = delete;

private:
	Networking::ClientServer::ClientServerAdapter adapter;
	::yojimbo::Server server;

	std::unordered_map<YojimboPlugin::ClientIndex_T, Networking::ClientServer::UnauthenticatedClient> unauthed_clients;
	std::unordered_map<YojimboPlugin::ClientIndex_T, Networking::ClientServer::ActiveClient> active_clients;
};
