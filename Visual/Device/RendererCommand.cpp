#include "RendererCommand.hpp"

#include "Window.hpp"
#include "RendererAPI.hpp"

namespace
{
	Visual::Device::Window* active_window;
}

namespace Visual::Device
{
	void RendererCommand::SetActiveContext( Window& window )
	{
		active_window = &window;
	}

	void RendererCommand::ClearActiveContext()
	{
		active_window = nullptr;
	}

	bool RendererCommand::IsValid()
	{
		return (active_window != nullptr);
	}

	Window& RendererCommand::GetActiveContext()
	{
		ASSERT( IsValid() );
		return *active_window;
	}

	void RendererCommand::SetViewport( uint32_t x, uint32_t y, uint32_t width, uint32_t height )
	{
		ASSERT( IsValid() );
		active_window->GetRendererAPI().SetViewport( x, y, width, height );
	}

	uint32_t RendererCommand::GetViewportX()
	{
		ASSERT( IsValid() );
		return active_window->GetRendererAPI().GetViewportX();
	}

	uint32_t RendererCommand::GetViewportY()
	{
		ASSERT( IsValid() );
		return active_window->GetRendererAPI().GetViewportY();
	}

	uint32_t RendererCommand::GetViewportWidth()
	{
		ASSERT( IsValid() );
		return active_window->GetRendererAPI().GetViewportWidth();
	}

	uint32_t RendererCommand::GetViewportHeight()
	{
		ASSERT( IsValid() );
		return active_window->GetRendererAPI().GetViewportHeight();
	}

	void RendererCommand::SetClearColour( ColourRGBA colour )
	{
		ASSERT( IsValid() );
		active_window->GetRendererAPI().SetClearColour( colour );
	}

	void RendererCommand::Clear()
	{
		ASSERT( IsValid() );
		active_window->GetRendererAPI().Clear();
	}

	void RendererCommand::DrawIndexed( const std::shared_ptr<VertexArray>& vertex_array, uint32_t count )
	{
		ASSERT( IsValid() );
		active_window->GetRendererAPI().DrawIndexed( vertex_array, count );
	}

	::RendererAPIs::API RendererCommand::GetAPIType()
	{
		ASSERT( IsValid() );
		return GetRendererAPI().GetAPIType();
	}

	RendererAPI& RendererCommand::GetRendererAPI()
	{
		ASSERT( IsValid() );
		return active_window->GetRendererAPI();
	}
}
