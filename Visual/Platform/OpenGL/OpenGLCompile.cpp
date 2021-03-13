#include "Precompiled/pch.hpp"
#include "OpenGLHeader.hpp"

#if defined(RENDERER_IMPLEMENTATION_OPENGL)

// link glew library
#	ifdef PLATFORM_WINDOWS
#		ifdef GLEW_STATIC
#			ifdef _WIN64
#				pragma comment(lib, "Visual/Vendor/glew/lib/Release/x64/glew32s.lib")
#			else
#				pragma comment(lib, "Visual/Vendor/glew/lib/Release/Win32/glew32s.lib")
#			endif
#		else
#			ifdef _WIN64
#				pragma comment(lib, "Visual/Vendor/glew/lib/Release/x64/glew32.lib")
#			else
#				pragma comment(lib, "Visual/Vendor/glew/lib/Release/Win32/glew32.lib")
#			endif
#		endif
#	endif

// link opengl
#	pragma comment(lib, "opengl32.lib")

// include the glew SOURCE file (actual file should remain excluded from project)
#	pragma warning(push, 0)
#		include "Visual/Vendor/glew/src/glew.c"
#	pragma warning(pop)

#endif

