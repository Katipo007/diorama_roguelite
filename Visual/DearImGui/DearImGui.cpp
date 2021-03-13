#include "Precompiled/pch.hpp"
#include "DearImGui.hpp"

#ifdef DEARIMGUI_ENABLED

namespace
{
	bool s_dearimgui_enabled = false;
}

namespace DearImGui
{
	bool IsEnabled()
	{
		return s_dearimgui_enabled;
	}

	void SetEnabled( const bool enabled )
	{
		s_dearimgui_enabled = enabled;
	}
}

#endif // DEARIMGUI_ENABLED
