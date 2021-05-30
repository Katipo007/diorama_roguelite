#pragma once

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

	namespace detail
	{
		using ActiveClientPredicate_T = std::function<bool( const ActiveClient& )>;
		const auto AllActiveClientsPredicate = []( const ActiveClient& ) { return true; };
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
		bool AcceptClient( UnauthenticatedClient& client );
		bool RejectClient( UnauthenticatedClient& client );

		void DisconnectClient( BaseClientConnection& client, std::optional<std::string> reason = std::nullopt );
		void DisconnectAllClients( std::optional<std::string> reason = std::nullopt );

		void OnFixedUpdate( const PreciseTimestep& ts );

		void ForEachClient( std::invocable<ActiveClient&> auto func, std::predicate<const ActiveClient&> auto predicate );
		void ForEachClient( std::invocable<const ActiveClient&> auto func, std::predicate<const ActiveClient&> auto predicate ) const;

		bool SendMessage( BaseClientConnection& client, ChannelType channel, YojimboPlugin::MessageType_T type, const std::function<void( yojimbo::Message& )>& initialiser );
		template<class T>
		bool SendMessage( BaseClientConnection& client, ChannelType channel, const std::function<void( T& )>& initialiser )
		{
			return SendMessage( client, channel, MessageFactory::template GetMessageType<T>(), [&]( yojimbo::Message& message ) { initialiser( static_cast<T&>(message) ); } );
		}

		size_t BroadcastMessage( ChannelType channel, YojimboPlugin::MessageType_T type, const std::function<void( ActiveClient&, yojimbo::Message& )>& initialiser, const detail::ActiveClientPredicate_T& inclusion_predicate = detail::AllActiveClientsPredicate );
		template<class T>
		bool BroadcastMessage( ChannelType channel, const std::function<void( ActiveClient&, T& )>& initialiser, const detail::ActiveClientPredicate_T& inclusion_predicate = detail::AllActiveClientsPredicate )
		{
			return BroadcastMessage( channel, MessageFactory::template GetMessageType<T>(), [&]( ActiveClient& client, yojimbo::Message& message ) { initialiser( client, static_cast<T&>(message) ); }, inclusion_predicate );
		}

		// Signals
		Signal::signal<ActiveClient&> ClientAccepted; // fired when a client is now active for play
		Signal::signal<ActiveClient&> ClientDisconnected; // fired when a previously accepted client is being disconnected

	private:
		void ClientConnectedHandler( ClientServerAdapter& adapter, YojimboPlugin::ClientIndex_T index );
		void ClientDisconnectedHandler( ClientServerAdapter& adapter, YojimboPlugin::ClientIndex_T index );

		void ProcessMessages();

		void ForAllConnections( std::invocable<BaseClientConnection&> auto func );
		BaseClientConnection* FindClientConnection( YojimboPlugin::ClientIndex_T client_index );

	private:
		GameServer( const GameServer& ) = delete;
		GameServer( GameServer&& ) = delete;
		GameServer& operator=( const GameServer& ) = delete;
		GameServer& operator=( GameServer&& ) = delete;

	private:
		ClientServerAdapter adapter;
		::yojimbo::Server server;

		std::unordered_map<YojimboPlugin::ClientIndex_T, UnauthenticatedClient> unauthed_clients;
		std::unordered_map<YojimboPlugin::ClientIndex_T, ActiveClient> active_clients;
	};
}
