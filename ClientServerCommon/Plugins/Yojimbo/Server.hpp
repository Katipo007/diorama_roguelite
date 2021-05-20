#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>

#include "Common/Utility/Timestep.hpp"
#include "Common/Utility/Signal.hpp"

#include "Types.hpp"

namespace Plugins { class YojimboPlugin; }

namespace YojimboPlugin
{
	class ClientConnection;
	struct Message;

	using ClientConnectionPredicate_T = std::function<bool( const ClientConnection& )>;
	inline static const ClientConnectionPredicate_T AllClientConnectionsPredicate = []( const ClientConnection& ) { return true; };

	class BaseServer
	{
		friend class ::Plugins::YojimboPlugin;

	public:
		virtual ~BaseServer() {}

		virtual bool IsRunning() const noexcept = 0;

		virtual size_t GetMaxConnectedClients() const noexcept = 0;
		virtual size_t GetNumConnectedClients() const noexcept = 0;

		[[nodiscard]] virtual bool IsDisconnecting( const ClientConnection& client ) const = 0;
		virtual void DisconnectClient( ClientConnection& client, std::optional<std::string> reason, bool sever = false ) = 0; // Mark a client to be disconnected next update, the `sever` argument will do it immediately but most likely won't be able to provide the reason

		[[nodiscard]] virtual bool OwnsConnection( const ClientConnection& client ) const noexcept = 0;
		[[nodiscard]] virtual ClientId_T GetClientId( const ClientConnection& client ) const = 0;
		[[nodiscard]] virtual ClientConnection* GetClientById( ClientId_T id ) noexcept = 0;
		[[nodiscard]] virtual const ClientConnection* GetClientById( ClientId_T id ) const noexcept = 0;

		[[nodiscard]] virtual ClientConnection* FindClient( const ClientConnectionPredicate_T& predicate ) = 0;
		[[nodiscard]] virtual const ClientConnection* FindClient( const ClientConnectionPredicate_T& predicate ) const = 0;
		virtual void ForEachClient( const std::function<void( ClientConnection& connection )>& func ) = 0;
		virtual void ForEachClient( const std::function<void( const ClientConnection& connection )>& func ) const = 0;

		virtual bool SendMessage( MessageType type, ChannelType channel, ClientConnection& client, const MessageInitialiserFunc_T& message_initialiser ) = 0;
		/// <summary>
		/// Send a message to all clients (with an optional predicate to specify which clients)
		/// </summary>
		virtual size_t BroadcastMessage( MessageType type, ChannelType channel, const MessageInitialiserFunc_T& message_initialiser, const ClientConnectionPredicate_T& predicate = AllClientConnectionsPredicate ) = 0;

	public: // Events
		sigslot::signal<BaseServer&, ClientConnection&> ClientConnected; // when a client connection is first established
		sigslot::signal<BaseServer&, ClientConnection&> ClientDisconnected; // when a client has been disconnected (from any state)

	protected:
		virtual void Update( const PreciseTimestep& timestep ) = 0;

	protected:
		bool HandleMessage( ClientConnection& connection, Message& message );
	};
}
