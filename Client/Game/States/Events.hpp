#pragma once

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

#include "Common/Utility/Timestep.hpp"

namespace API { class DearImGuiAPI; }

namespace Game::Networking { class ClientServerSession; }

namespace Game::Events
{
	struct FrameEvent
	{
		explicit FrameEvent( PreciseTimestep step )
			: precise_timestep( step )
			, timestep( step )
		{}

		PreciseTimestep precise_timestep;
		Timestep timestep;
	};

	struct RenderEvent { };

	struct DearImGuiFrameEvent
	{
		explicit DearImGuiFrameEvent( ::API::DearImGuiAPI& dearimgui_ )
			: dearimgui_api( dearimgui_ )
		{}

		::API::DearImGuiAPI& dearimgui_api;
	};

	struct ConnectedToServerEvent
	{
		explicit ConnectedToServerEvent( Networking::ClientServerSession& session )
			: session( session )
		{}

		Networking::ClientServerSession& session;
	};

	struct ConnectingToServerEvent
	{
		explicit ConnectingToServerEvent( Networking::ClientServerSession& session )
			: session( session )
		{}

		Networking::ClientServerSession& session;
	};

	struct DisconnectedFromServerEvent
	{
		explicit DisconnectedFromServerEvent( Networking::ClientServerSession& session, std::optional<std::string> reason = std::nullopt )
			: session( session )
			//, reason( reason )
		{
			(void)reason;
		}

		Networking::ClientServerSession& session;
		//std::optional<std::string> reason;
	};
}
