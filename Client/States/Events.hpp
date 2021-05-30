#pragma once

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

#include "Common/Utility/Timestep.hpp"

namespace yojimbo { class Message; }

namespace API { class DearImGuiAPI; }

namespace Networking::ClientServer { class ServerConnection; }

namespace ClientStates
{
	struct FrameEvent
	{
		explicit FrameEvent( PreciseTimestep step );

		PreciseTimestep precise_timestep;
		Timestep timestep;
	};

	struct RenderEvent { };

	struct DearImGuiFrameEvent
	{
		explicit DearImGuiFrameEvent( ::API::DearImGuiAPI& dearimgui_ );

		::API::DearImGuiAPI& dearimgui_api;
	};

	struct ConnectedToServerEvent
	{
		explicit ConnectedToServerEvent( Networking::ClientServer::ServerConnection& connection );

		Networking::ClientServer::ServerConnection& connection;
	};

	struct ConnectingToServerEvent
	{
		explicit ConnectingToServerEvent( Networking::ClientServer::ServerConnection& connection );

		Networking::ClientServer::ServerConnection& connection;
	};

	struct DisconnectedFromServerEvent
	{
		explicit DisconnectedFromServerEvent( Networking::ClientServer::ServerConnection& connection, std::string_view reason = "" );

		Networking::ClientServer::ServerConnection& connection;
		std::array<char, 128> reason;
	};

	struct ServerMessageEvent
	{
		explicit ServerMessageEvent( Networking::ClientServer::ServerConnection& connection, const yojimbo::Message& message );

		void FlagAsHandled() const;

		Networking::ClientServer::ServerConnection& connection;
		const yojimbo::Message& message;
		bool handled = false;
	};
}
