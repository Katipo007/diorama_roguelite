#pragma once

#ifdef PLATFORM_WINDOWS
#	include "Visual/Platform/Windows/WindowsHeader.hpp"
#	define GLEW_STATIC

#	pragma warning(push, 0) // This ignores all warnings raised inside External headers
#		include "Visual/Vendor/glew/include/GL/glew.h"
#		include "Visual/Vendor/glew/include/GL/wglew.h"
#	pragma warning(pop)
#else
#	error OpenGL includes not specified for this platform
#endif