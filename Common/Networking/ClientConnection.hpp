#pragma once

#include <time.h>

#include "ChannelTypesEnum.hpp"
#include "Types.hpp"

namespace Networking
{
	class Message;
	class Server;

	/// <summary>
	/// Server-side
	/// Represents a single client's connection to the server
	/// </summary>
	class ClientConnection
	{
	public:
		ClientConnection( Server& owner );
		virtual ~ClientConnection() {}

		Server& GetOwner() const noexcept { return owner; }
		[[nodiscard]] ClientId_T GetId() const noexcept;
		time_t GetConnectedTimestamp() const noexcept { return connected_at; }

		virtual void SendMessage( std::unique_ptr<Message> message, ChannelType channel );
		virtual bool ProcessMessage( const Message& message );

	private:
		Server& owner;
		const time_t connected_at;
	};
}
