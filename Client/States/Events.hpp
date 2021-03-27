#pragma once

#include <memory>
#include <optional>

#include "Client/Sessions/ClientServerSession.hpp"
#include "Utility/Timestep.hpp"

namespace ClientStates
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

	struct RenderEvent {};
	struct DearImGuiFrameEvent {};

	struct ConnectedToServerEvent
	{
		explicit ConnectedToServerEvent( std::unique_ptr<Sessions::ClientServerSession> session )
			: session( std::move( session ) )
		{}

		std::unique_ptr<Sessions::ClientServerSession> session;
	};

	struct DisconnectedFromServerEvent
	{
		explicit DisconnectedFromServerEvent() {}

		std::optional<std::string> reason;
	};
}
