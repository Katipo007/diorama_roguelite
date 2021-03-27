#pragma once

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
}
