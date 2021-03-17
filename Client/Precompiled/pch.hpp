#pragma once

#ifndef DIORAMA_ROGUELITE_PCH_HPP
#define DIORAMA_ROGUELITE_PCH_HPP

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Common/Core/Base.hpp"
#include "Common/Core/Logging.hpp"

#if defined(PLATFORM_WINDOWS) 
#	define RENDERER_IMPLEMENTATION_OPENGL
#endif

#define GLM_FORCE_SILENT_WARNINGS

#pragma warning(disable: 4201)

#endif // DIORAMA_ROGUELITE_PCH_HPP