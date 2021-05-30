#pragma once

#pragma warning(push, 0)
#include "ClientServerCommon/Vendor/yojimbo/yojimbo.h"
#pragma warning(pop)

#ifdef _WIN64
#	ifdef _DEBUG
#		pragma comment(lib, "ClientServerCommon/Vendor/yojimbo/bin/x64/Debug/yojimbo.lib")
#	else
#		pragma comment(lib, "ClientServerCommon/Vendor/yojimbo/bin/x64/Release/yojimbo.lib")
#	endif
#else
#error Haven't setup lib paths for yojimbo.lib on x86 (if yojimbo even supports x86)
#endif

