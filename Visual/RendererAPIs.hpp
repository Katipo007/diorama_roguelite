#pragma once

namespace RendererAPIs
{
	enum class API
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

	static_assert((size_t)API::NumAPIs > 0, "Platform has no supported graphic APIs");
}
