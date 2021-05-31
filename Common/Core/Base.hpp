#pragma once

#include "Common/CompileTime/PlatformDetection.hpp"

#ifdef _DEBUG // TODO: replace with a custom define
#	if defined( PLATFORM_WINDOWS )
#		define DEBUGBREAK() __debugbreak()
#	else
#		error "Platform does not support debugbreak yet!"
#	endif
#	define ENABLE_ASSERTS
#else
#	define DEBUGBREAK() (void)0
#endif

#define EXPAND_MACRO(x) x
#define STRINGIFY_MACRO(x) #x

#define BIT(x) ( 1 << x )

// these are at the bottom because these files reference this one
#include "Common/Deployment/Deployment.hpp"
#include "Common/Core/Logging.hpp"
#include "Common/Core/Assert.hpp"
