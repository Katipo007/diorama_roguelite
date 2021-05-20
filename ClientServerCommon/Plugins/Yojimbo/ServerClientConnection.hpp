#pragma once

#include <functional>

#include "ClientServerCommon/Plugins/Yojimbo/Types.hpp"

namespace yojimbo
{
	class BaseServer;
	class Message;
}

namespace YojimboPlugin
{
	/// <summary>
	/// Represents a connection between the server and a single client
	/// </summary>
	class ServerClientConnection final
	{
	public:
		using MessageHandlerCallback_T = std::function<bool( ServerClientConnection& connection, yojimbo::Message& )>;

	public:
		ServerClientConnection( yojimbo::BaseServer& server, ClientIndex_T index );
		~ServerClientConnection();

		bool IsConnected() const;
		ClientIndex_T GetClientIndex() const noexcept { return index; }
		ClientId_T GetId() const noexcept;

		void Disconnect();

		bool SendMessage( ChannelIndex_T channel, yojimbo::Message* message );
		bool CanSendMessage( ChannelIndex_T channel ) const;

		void ProcessMessages( ChannelIndex_T channel, const MessageHandlerCallback_T& handler );

		// whether this connection is for a local player playing on an internal server
		bool IsLoopbackClient() const;

	private:
		yojimbo::BaseServer& server;
		const ClientIndex_T index;
	};
}
