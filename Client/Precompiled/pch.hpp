#pragma once

#ifndef DIORAMA_ROGUELITE_CLIENT_PCH_HPP
#define DIORAMA_ROGUELITE_CLIENT_PCH_HPP

#include "Common/Precompiled/pch.hpp"
#include "Client/Logging.hpp"
#include "Visual/Logging.hpp"

#if defined(PLATFORM_WINDOWS) 
#	define RENDERER_IMPLEMENTATION_OPENGL
#endif

#endif // DIORAMA_ROGUELITE_CLIENT_PCH_HPP