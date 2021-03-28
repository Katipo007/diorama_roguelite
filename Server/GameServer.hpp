#pragma once

#include <optional>

#include "ServerTypedefs.hpp"
#include "ClientServerCommon/Vendor/Wrappers/Networking.hpp"
#include "ClientServerCommon/Networking/MessageId.hpp"
#include "ClientServerCommon/Networking/ClientServerConfig.hpp"

#include "Common/Utility/Timestep.hpp"
#include "Common/Utility/NonCopyable.hpp"

namespace Server
{
	namespace Sessions
	{
		class ActiveClientSession;
	}

	class GameServer
		: NonCopyable
	{
		struct ConnectionAdapter final : public yojimbo::Adapter
		{
			explicit ConnectionAdapter( GameServer& server )
				: server( server )
			{}

			yojimbo::MessageFactory* CreateMessageFactory( yojimbo::Allocator& allocator ) override;

			void OnServerClientConnected( int client_index ) override;
			void OnServerClientDisconnected( int client_index ) override;

			GameServer& server;
		};
		using ClientContainer_T = std::unordered_map<ClientId_T, std::unique_ptr<Sessions::ActiveClientSession>>;

	public:
		GameServer( const yojimbo::Address& server_address, const uint16_t max_connected_clients = 6 );
		virtual ~GameServer();

		virtual void OnClientConnected( ClientIndex_T client_index ); ///< called via the adapter
		virtual void OnClientDisconnected( ClientIndex_T client_index ); ///< called via the adapter

		virtual void Update( const PreciseTimestep& timestep );

		ClientId_T GetClientIdByIndex( const ClientIndex_T& client_index ) const;
		ClientIndex_T GetClientIndexById( const ClientId_T& client_id ) const;

		Sessions::ActiveClientSession* GetClientById( const ClientId_T& client_id );
		const Sessions::ActiveClientSession* GetClientById( const ClientId_T& client_id ) const;
		Sessions::ActiveClientSession* GetClientByIndex( const ClientIndex_T& client_id );
		const Sessions::ActiveClientSession* GetClientByIndex( const ClientIndex_T& client_id ) const;

		void ForEachClient( const std::function<void( Sessions::ActiveClientSession& )>& func );
		void ForEachClient( const std::function<void( const Sessions::ActiveClientSession& )>& func ) const;

		void DisconnectClient( Sessions::ActiveClientSession& session, std::optional<std::string> reason );
		void SeverClient( Sessions::ActiveClientSession& session ); // Disconnects client imediately without reason or warning

		/// <summary>
		/// Send a message to a single client
		/// </summary>
		/// <param name="session"></param>
		/// <param name="message_id"></param>
		/// <param name="channel"></param>
		/// <param name="message_init_func"></param>
		bool SendMessage(
			const Sessions::ActiveClientSession& session
			, const Networking::MessageId::MessageId message_id
			, const Networking::ChannelType::Type channel
			, const std::function<void( yojimbo::Message&, const Sessions::ActiveClientSession& )>& message_init_func
			);

		/// <summary>
		/// Send a message to all clients (with an optional predicate to specify which clients)
		/// </summary>
		/// <param name="message_id"></param>
		/// <param name="channel"></param>
		/// <param name="message_init_func"></param>
		/// <param name="inclusion_predicate"></param>
		/// <returns>Number of clients the message was sent to</returns>
		size_t BroadcastMessage(
			const Networking::MessageId::MessageId message_id
			, const Networking::ChannelType::Type channel
			, const std::function<void( yojimbo::Message&, const Sessions::ActiveClientSession& )>& message_init_func
			, const std::function<bool( const Sessions::ActiveClientSession& )> inclusion_predicate = []( const Sessions::ActiveClientSession& ) { return true; }
		);

	protected:
		virtual void ProcessMessages();

	private:
		bool OwnsClient( const Sessions::ActiveClientSession& session ) const;

	private:
		ConnectionAdapter adapter;
		Networking::ClientServerGameConnectionConfig connection_config;
		yojimbo::Server server;
		const uint16_t max_connected_clients;

		ClientContainer_T connected_clients;
	};
}
