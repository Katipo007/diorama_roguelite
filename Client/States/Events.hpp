#pragma once

#include "Utility/Timestep.hpp"

namespace ClientStates
{
	struct FrameEvent
	{
		explicit FrameEvent( Timestep step )
			: timestep( step )
		{}

		Timestep timestep;
	};

	struct RenderEvent {};
	struct DearImGuiFrameEvent {};
}
