#include "DearImGui.hpp"

#ifdef DEARIMGUI_ENABLED

namespace
{
	static bool s_dearimgui_enabled = true; // TODO: disabled by default once we aren't using DearImGui for all our UI
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
