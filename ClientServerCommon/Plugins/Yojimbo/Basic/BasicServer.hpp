#pragma once

#include "../Server.hpp"
#include "../ClientConnection.hpp"

#include "BasicAdapter.hpp"
#include "../impl/YojimboHeader.hpp"

namespace Plugins { class YojimboPlugin; }

namespace YojimboPlugin
{
	class BasicServer final
		: public Server
	{
		friend class ::Plugins::YojimboPlugin;

		struct ClientEntry
		{
			const ClientId_T id;
			const int yojimbo_index;

			bool is_disconnecting = false;
			ClientConnection connection;

			ClientEntry( int yojimbo_index, ClientId_T id, ClientConnection&& connection );
		};
		using ClientContainer_T = std::unordered_map<ClientId_T, ClientEntry>;

	public:
		BasicServer(
			yojimbo::Address&& host_address
			, size_t max_connected_clients
			, const std::array<uint8_t, yojimbo::KeyBytes>& private_key
			, yojimbo::ClientServerConfig&& config
			, BasicAdapter&& adapter
		);
		~BasicServer();

		bool IsRunning() const noexcept override;

		size_t GetMaxConnectedClients() const noexcept override;
		size_t GetNumConnectedClients() const noexcept override;

		virtual bool IsDisconnecting( const ClientConnection& client ) const override;
		void DisconnectClient( ClientConnection& client, std::optional<std::string> reason, bool sever = false ) override;

		bool OwnsConnection( const ClientConnection& client ) const noexcept override;
		ClientId_T GetClientId( const ClientConnection& client ) const override;
		ClientConnection* GetClientById( ClientId_T id ) noexcept override;
		const ClientConnection* GetClientById( ClientId_T id ) const noexcept override;

		ClientConnection* FindClient( const ClientConnectionPredicate_T& predicate ) override { return const_cast<ClientConnection*>( ((const BasicServer*)this)->FindClient( predicate )); }
		const ClientConnection* FindClient( const ClientConnectionPredicate_T& predicate ) const override;
		void ForEachClient( const std::function<void( ClientConnection& connection )>& func ) override;
		void ForEachClient( const std::function<void( const ClientConnection& connection )>& func ) const override;

		bool SendMessage( MessageType type, ChannelType channel, ClientConnection& client, const MessageInitialiserFunc_T& initialiser ) override;
		size_t BroadcastMessage( MessageType type, ChannelType channel, const MessageInitialiserFunc_T& initialiser, const ClientConnectionPredicate_T& predicate = AllClientConnectionsPredicate ) override;

	private:
		void Update( const PreciseTimestep& timestep ) override;

		[[nodiscard]] const ClientEntry* FindClientEntryById( ClientId_T client_id ) const;
		[[nodiscard]] ClientEntry* FindClientEntryById( ClientId_T client_id ) { return const_cast<ClientEntry*>(((const BasicServer*)this)->FindClientEntryById( client_id )); }
		[[nodiscard]] const ClientEntry* FindClientEntryByIndex( int client_index ) const;
		[[nodiscard]] ClientEntry* FindClientEntryByIndex( int client_index ) { return const_cast<ClientEntry*>(((const BasicServer*)this)->FindClientEntryByIndex( client_index )); }
		[[nodiscard]] const ClientEntry* FindClientEntry( const ClientConnection& client ) const;
		[[nodiscard]] ClientEntry* FindClientEntry( const ClientConnection& client ) { return const_cast<ClientEntry*>(((const BasicServer*)this)->FindClientEntry( client )); }

		void ProcessMessages();

	private:
		ClientContainer_T clients;

		const yojimbo::ClientServerConfig config;
		BasicAdapter adapter;
		yojimbo::Server server;
	};
}
