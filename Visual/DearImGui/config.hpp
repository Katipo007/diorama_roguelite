#pragma once

#include "Common/Core/Assert.hpp"

#ifndef _DEBUG
#	define IMGUI_DISABLE_DEMO_WINDOWS
#endif

//---- Define assertion handler. Defaults to calling assert().
// If your macro uses multiple statements, make sure is enclosed in a 'do { .. } while (0)' block so it can be used as a single statement.
//#define IM_ASSERT(_EXPR)  ASSERT(_EXPR)

//---- Don't define obsolete functions/enums/behaviors. Consider enabling from time to time after updating to avoid using soon-to-be obsolete function/names.
#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS

//---- Debug Tools: Macro to break in Debugger
// (use 'Metrics->Tools->Item Picker' to pick widgets with the mouse and break into them for easy debugging.)
#define IM_DEBUG_BREAK  DEBUGBREAK

