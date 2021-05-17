#pragma once

#include <time.h>
#include <optional>

#include "Types.hpp"

namespace YojimboPlugin
{
	struct Message;
	class Server;

	/// <summary>
	/// Server-side
	/// Represents a single client's connection to the server
	/// </summary>
	class ClientConnection
	{
		friend class Server;

	public:
		using MessageHandler_T = std::function<bool( const Message& )>;

	public:
		ClientConnection( Server& owner, ClientId_T id );
		virtual ~ClientConnection() {}

		Server& GetOwner() const noexcept { return owner; }
		ClientId_T GetId() const noexcept { return id; }
		time_t GetConnectedTimestamp() const noexcept { return connected_at; }

		bool IsDisconnecting() const;
		void Disconnect( std::optional<std::string> reason );

		bool HasMessageHandler() const noexcept { return (bool)message_handler; }
		void SetMessageHandler( MessageHandler_T handler ) noexcept { message_handler = handler; }

		virtual void SendMessage( MessageType message, ChannelType channel, const MessageInitialiserFunc_T& initialiser );

	private:
		Server& owner;
		const ClientId_T id;
		const time_t connected_at;
		MessageHandler_T message_handler;
	};
}
