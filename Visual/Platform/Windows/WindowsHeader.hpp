#pragma once

#if defined(PLATFORM_WINDOWS)
#	ifndef NOMINMAX
// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
#		define NOMINMAX
#	endif

#	include <Windows.h>
#else
#	warning Visual/Platform/Windows/WindowsHeader.hpp is being included on a non-windows platform
#endif
