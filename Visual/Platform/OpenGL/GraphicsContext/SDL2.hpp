#pragma once

#include "Visual/Device/GraphicsContext.hpp"

#ifdef RENDERER_IMPLEMENTATION_OPENGL

struct SDL_Window;
typedef void* SDL_GLContext;

namespace Visual::Device::OpenGL
{
	class GraphicsContextOpenGL_SDL2
		: public GraphicsContext
	{
	public:
		GraphicsContextOpenGL_SDL2( SDL_Window* window_handle );

		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void FrameBegin() override;
		virtual void FrameEnd() override;
		virtual void SwapBuffers() override;

	private:
		SDL_Window* const window_handle;
		SDL_GLContext gl_context;
	};
}

#endif
