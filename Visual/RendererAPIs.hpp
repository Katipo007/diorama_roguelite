#pragma once

namespace RendererAPIs
{
	enum API
	{
#ifdef RENDERER_IMPLEMENTATION_NONE
		None,
#endif

#ifdef RENDERER_IMPLEMENTATION_OPENGL
		OpenGL,
#endif

#ifdef RENDERER_IMPLEMENTATION_VULKAN
		Vulkan,
#endif

		NumAPIs
	};

	static_assert(API::NumAPIs > 0, "Platform has no supported graphic APIs");
}
