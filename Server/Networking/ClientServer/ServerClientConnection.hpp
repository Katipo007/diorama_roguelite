#pragma once

#include <functional>
#include <optional>
#include <time.h>

#include "ClientServerCommon/Networking/ClientServer/Config.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/Types.hpp"
#include "Common/Utility/Timestep.hpp"

namespace yojimbo
{
	class Server;
	class Message;
}

class GameServer;

namespace Networking::ClientServer
{
	/// Represents a connection between the server and a single client
	class ServerClientConnection final
	{
		friend class GameServer;

	public:
		using MessageHandlerCallback_T = std::function<bool( ServerClientConnection& connection, const yojimbo::Message& )>;

	public:
		ServerClientConnection( yojimbo::Server& server, YojimboPlugin::ClientIndex_T index ) noexcept;
		~ServerClientConnection() noexcept;

		bool IsConnected() const noexcept;
		bool Expired() const noexcept { return !IsConnected(); }

		bool CanSendMessage( ChannelType channel ) const noexcept;

		YojimboPlugin::ClientIndex_T GetClientIndex() const noexcept { return client_index; }
		YojimboPlugin::ClientId_T GetId() const noexcept;

		bool IsLoopbackClient() const noexcept; // whether this connection is for a local player playing on an internal server

		void OnFixedUpdate( const PreciseTimestep& ts );
		void Disconnect( std::optional<std::string> reason = std::nullopt );

		void SetMessageHandler( MessageHandlerCallback_T callback );
		void ResetMessageHandler() { SetMessageHandler( {} ); }

	private:
		void ProcessMessages();
		bool SendMessageInternal( ChannelType channel, yojimbo::Message* message );
		void FlushMessages();

	private:
		yojimbo::Server& server;
		const YojimboPlugin::ClientIndex_T client_index;
		const time_t connected_at_timestamp;

		bool being_destroyed_from_disconnect_handler = false; // For GameServer to set

		MessageHandlerCallback_T message_handler;
	};
}
