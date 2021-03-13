#pragma once

#include "Visual/Graphics/Colour.hpp"
#include "RendererAPIs.hpp"
#include "VertexArray.hpp"

namespace Visual::Device
{
	class Window;
	class RendererAPI;

	class RendererCommand
	{
	public:
		static void SetActiveContext( Window& window );
		static void ClearActiveContext();
		static bool IsValid();
		static Window& GetActiveContext();

		static void SetViewport( uint32_t x, uint32_t y, uint32_t width, uint32_t height );
		static uint32_t GetViewportX();
		static uint32_t GetViewportY();
		static uint32_t GetViewportWidth();
		static uint32_t GetViewportHeight();

		static void SetClearColour( ColourRGBA colour );
		static void Clear();

		static void DrawIndexed( const std::shared_ptr<VertexArray>& vertex_array, uint32_t count = 0 );

		static ::RendererAPIs::API GetAPIType();
		static RendererAPI& GetRendererAPI();
	};
}
