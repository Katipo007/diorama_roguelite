#pragma once

#include <memory>
#include <optional>
#include <string>
#include <type_traits>

#include "Utility/Timestep.hpp"

namespace API { class DearImGuiAPI; }
namespace YojimboPlugin { class Client; }

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
		explicit ConnectedToServerEvent( YojimboPlugin::Client& session )
			: session( session )
		{
		}

		YojimboPlugin::Client& session;
	};

	struct DisconnectedFromServerEvent
	{
		static const size_t MaxReasonLength = 128;

		explicit DisconnectedFromServerEvent( YojimboPlugin::Client& session, std::string_view reason = "" )
			: session( session )
		{
			ASSERT( reason.length() < MaxReasonLength );
			std::copy_n( reason.begin(), std::min( reason.length(), MaxReasonLength - 1 ), this->reason );
			this->reason[MaxReasonLength - 1] = '\n';
		}

		YojimboPlugin::Client& session;
		char reason[MaxReasonLength];
	};
}
