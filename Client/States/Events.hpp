#pragma once

#include <memory>
#include <optional>
#include <string>
#include <type_traits>

#include "Utility/Timestep.hpp"

namespace API { class DearImGuiAPI; }

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

	struct RenderEvent
	{
	};

	struct DearImGuiFrameEvent
	{
		explicit DearImGuiFrameEvent( ::API::DearImGuiAPI& dearimgui_ )
			: dearimgui_api( dearimgui_ )
		{}

		::API::DearImGuiAPI& dearimgui_api;
	};

	struct ConnectedToServerEvent
	{
		explicit ConnectedToServerEvent()
		{
		}
	};

	struct DisconnectedFromServerEvent
	{
		explicit DisconnectedFromServerEvent(std::string_view reason = "" )
			: reason( reason )
		{
		}

		std::string reason;
	};
}
