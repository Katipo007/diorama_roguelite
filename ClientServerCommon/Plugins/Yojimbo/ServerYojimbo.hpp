#pragma once

#include "Common/Networking/Server.hpp"

#include "YojimboHeader.hpp"
#include "YojimboNetworkAdapters.hpp"
#include "ClientConnectionYojimbo.hpp"

namespace Networking { class IMessageFactory; }

namespace Plugins { class NetworkYojimbo; }

namespace Plugins::Network::Yojimbo
{
	class ServerYojimbo final
		: public ::Networking::Server
	{
		friend class ::Plugins::NetworkYojimbo;

		struct ClientEntry
		{
			const ::Networking::ClientId_T id;
			const int yojimbo_index;

			bool is_disconnecting = false;
			::Networking::ClientConnectionState state;
			ClientConnectionYojimbo connection;

			ClientEntry( int yojimbo_index, ::Networking::ClientId_T id, ClientConnectionYojimbo&& connection );
		};
		using ClientContainer_T = std::unordered_map<::Networking::ClientId_T, ClientEntry>;

	public:
		ServerYojimbo(
			yojimbo::Address&& host_address
			, size_t max_connected_clients
			, const std::array<uint8_t, yojimbo::KeyBytes>& private_key
			, yojimbo::ClientServerConfig&& config
			, ServerAdapter&& adapter
		);
		~ServerYojimbo();

		bool IsRunning() const noexcept override;

		size_t GetMaxConnectedClients() const noexcept override;
		size_t GetNumConnectedClients() const noexcept override;

		::Networking::ClientConnectionState GetClientState( const ::Networking::ClientConnection& client ) const override;
		void IncrementClientState( ::Networking::ClientConnection& client, ::Networking::ClientConnectionState new_state ) override;
		void DisconnectClient( ::Networking::ClientConnection& client, std::optional<std::string_view> reason, bool sever = false ) override;

		bool OwnsConnection( const ::Networking::ClientConnection& client ) const noexcept override;
		::Networking::ClientId_T GetClientId( const ::Networking::ClientConnection& client ) const override;
		::Networking::ClientConnection* GetClientById( ::Networking::ClientId_T id ) noexcept override;
		const ::Networking::ClientConnection* GetClientById( ::Networking::ClientId_T id ) const noexcept override;

		::Networking::ClientConnection* FindClient( ::Networking::ClientConnectionState state, const std::function<bool( const ::Networking::ClientConnection& )>& predicate ) override;
		const ::Networking::ClientConnection* FindClient( ::Networking::ClientConnectionState state, const std::function<bool( const ::Networking::ClientConnection& )>& predicate ) const override;
		void ForEachClient( ::Networking::ClientConnectionState state, const std::function<void( ::Networking::ClientConnection& connection )>& func ) override;
		void ForEachClient( ::Networking::ClientConnectionState state, const std::function<void( const ::Networking::ClientConnection& connection )>& func ) const override;
		void ForEachClient( const std::function<void( ::Networking::ClientConnection& connection, ::Networking::ClientConnectionState state )>& func ) override;
		void ForEachClient( const std::function<void( const ::Networking::ClientConnection& connection, ::Networking::ClientConnectionState state )>& func ) const override;

		bool SendMessage( std::unique_ptr<::Networking::Message> message, ::Networking::ChannelType channel, ::Networking::ClientConnection& client ) override;
		size_t BroadcastMessage( std::unique_ptr<::Networking::Message> message, ::Networking::ChannelType channel, const std::function<bool( const ::Networking::ClientConnection& )>& predicate = []( const auto& ) { return true; } ) override;

	private:
		void Update( const PreciseTimestep& timestep ) override;

		[[nodiscard]] ClientEntry* FindClientEntryById( ::Networking::ClientId_T client_id );
		[[nodiscard]] const ClientEntry* FindClientEntryById( ::Networking::ClientId_T client_id ) const;
		[[nodiscard]] ClientEntry* FindClientEntryByIndex( int client_index );
		[[nodiscard]] const ClientEntry* FindClientEntryByIndex( int client_index ) const;
		[[nodiscard]] ClientEntry* FindClientEntry( const ::Networking::ClientConnection& client );
		[[nodiscard]] const ClientEntry* FindClientEntry( const ::Networking::ClientConnection& client ) const;

		void ProcessMessages();

	private:
		ClientContainer_T clients;

		ServerAdapter adapter;
		yojimbo::ClientServerConfig config;
		yojimbo::Server server;
	};
}
