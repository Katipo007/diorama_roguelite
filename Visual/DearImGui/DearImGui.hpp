#pragma once

#ifdef DEARIMGUI_ENABLED

#define IMGUI_USER_CONFIG "DearImGui/config.hpp"

#pragma warning(push, 0)
#include "Visual/Vendor/dearimgui/imgui.h"
#pragma warning(pop)

namespace DearImGui
{
	bool IsEnabled();
	void SetEnabled( const bool enabled );
}

#endif
