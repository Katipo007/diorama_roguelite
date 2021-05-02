#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>

#include "Common/Utility/Timestep.hpp"
#include "Common/Utility/Signal.hpp"

#include "Address.hpp"
#include "ClientConnectionStateEnum.hpp"
#include "ChannelTypesEnum.hpp"
#include "Types.hpp"

namespace API { class NetworkAPI; }

namespace Networking
{
	class ClientConnection;

	class Message;
	using ServerMessageHandlerFunc_T = std::function<void( ClientConnection&, const Message& )>;
	class IMessageFactory;

	struct ServerProperties
	{
		Address host_address;
		Key_T private_key;
		size_t max_num_clients;
		std::unique_ptr<IMessageFactory> message_factory;
		ServerMessageHandlerFunc_T message_handler_func;

		ServerProperties();
		~ServerProperties();
	};

	class Server
	{
		friend class API::NetworkAPI;

	public:
		virtual ~Server() {}

		virtual bool IsRunning() const noexcept = 0;

		virtual size_t GetMaxConnectedClients() const noexcept = 0;
		virtual size_t GetNumConnectedClients() const noexcept = 0;

		[[nodiscard]] virtual ClientConnectionState GetClientState( const ClientConnection& client ) const = 0;
		virtual void IncrementClientState( ClientConnection& client, ClientConnectionState new_state ) = 0;
		virtual void DisconnectClient( ClientConnection& client, std::optional<std::string_view> reason, bool sever = false ) = 0; // Mark a client to be disconnected next update, the `sever` argument will do it immediately but most likely won't be able to provide the reason

		[[nodiscard]] virtual bool OwnsConnection( const ClientConnection& client ) const noexcept = 0;
		[[nodiscard]] virtual ClientId_T GetClientId( const ClientConnection& client ) const = 0;
		[[nodiscard]] virtual ClientConnection* GetClientById( ClientId_T id ) noexcept = 0;
		[[nodiscard]] virtual const ClientConnection* GetClientById( ClientId_T id ) const noexcept = 0;

		[[nodiscard]] virtual ClientConnection* FindClient( ClientConnectionState state, const std::function<bool( const ClientConnection& )>& predicate ) = 0;
		[[nodiscard]] virtual const ClientConnection* FindClient( ClientConnectionState state, const std::function<bool( const ClientConnection& )>& predicate ) const = 0;
		virtual void ForEachClient( ClientConnectionState state, const std::function<void( ClientConnection& connection )>& func ) = 0;
		virtual void ForEachClient( ClientConnectionState state, const std::function<void( const ClientConnection& connection )>& func ) const = 0;
		virtual void ForEachClient( const std::function<void( ClientConnection& connection, ClientConnectionState state )>& func ) = 0;
		virtual void ForEachClient( const std::function<void( const ClientConnection& connection, ClientConnectionState state )>& func ) const = 0;

		virtual bool SendMessage( std::unique_ptr<Message> message, ChannelType channel, ClientConnection& client ) = 0;
		/// <summary>
		/// Send a message to all clients (with an optional predicate to specify which clients)
		/// </summary>
		virtual size_t BroadcastMessage( std::unique_ptr<Message> message, ChannelType channel, const std::function<bool( const ClientConnection& )>& predicate = []( const auto& ) { return true; } ) = 0;

	public: // Events
		sigslot::signal<Server&, ClientConnection&> ClientIntroduced; // when a client connection is first established
		sigslot::signal<Server&, ClientConnection&> ClientAuthenticated; // when a client has been successfully authenticated
		sigslot::signal<Server&, ClientConnection&> ClientAccepted; // when a client has been accepted for in-game play
		sigslot::signal<Server&, ClientConnection&, ClientConnectionState> ClientDisconnected; // when a client has been disconnected (from any state)

	protected:
		virtual void Update( const PreciseTimestep& timestep ) = 0;
	};
}
