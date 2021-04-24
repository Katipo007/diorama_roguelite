#include "WindowSDL2.hpp"

#include <SDL2/SDL.h>
#ifdef PLATFORM_WINDOWS
#	include <SDL2/SDL_syswm.h>
#endif

namespace Graphics::API
{
	WindowSDL2::WindowSDL2( SDL_Window* sdl_window_ )
		: sdl_window( sdl_window_ )
	{
		ASSERT( sdl_window != nullptr );
	}

	WindowSDL2::~WindowSDL2()
	{
	}

	void WindowSDL2::Destroy()
	{
		SDL_HideWindow( sdl_window );
		SDL_DestroyWindow( sdl_window );
		sdl_window = nullptr;
	}

	Size<uint32_t> WindowSDL2::GetSize() const
	{
		int w, h;
		SDL_GetWindowSize( sdl_window, &w, &h );
		return { (uint32_t)w, (uint32_t)h };
	}

	void WindowSDL2::SetSize( const Size<uint32_t> new_size )
	{
		SDL_SetWindowSize( sdl_window, new_size.width, new_size.height );
	}

	void WindowSDL2::SetMode( const WindowMode new_mode )
	{
		switch (new_mode)
		{
		case WindowMode::Window:
			SDL_SetWindowBordered( sdl_window, SDL_TRUE );
			SDL_SetWindowFullscreen( sdl_window, 0 );
			break;

		case WindowMode::Fullscreen:
			SDL_SetWindowBordered( sdl_window, SDL_TRUE );
			SDL_SetWindowFullscreen( sdl_window, SDL_WINDOW_FULLSCREEN );
			break;

		case WindowMode::BorderlessFullscreen:
			SDL_SetWindowBordered( sdl_window, SDL_FALSE );
			SDL_SetWindowFullscreen( sdl_window, SDL_WINDOW_FULLSCREEN_DESKTOP );
			break;
		}
	}

	WindowMode WindowSDL2::GetMode() const
	{
		auto flags = SDL_GetWindowFlags( sdl_window );

		if ((flags & SDL_WINDOW_FULLSCREEN_DESKTOP) && (flags & SDL_WINDOW_BORDERLESS))
			return WindowMode::BorderlessFullscreen;
		else if (flags & SDL_WINDOW_FULLSCREEN)
			return WindowMode::Fullscreen;

		return WindowMode::Window;
	}

	void WindowSDL2::SetState( const WindowState new_state )
	{
		switch (new_state)
		{
		case WindowState::Maximised:
			SDL_MaximizeWindow( sdl_window );
			break;

		case WindowState::Minimised:
			SDL_MinimizeWindow( sdl_window );
			break;

		case WindowState::Normal:
			SDL_RestoreWindow( sdl_window );
			break;
		}
	}

	WindowState WindowSDL2::GetState() const
	{
		auto flags = SDL_GetWindowFlags( sdl_window );
		
		if (flags & SDL_WINDOW_MINIMIZED)
			return WindowState::Minimised;
		else if (flags & SDL_WINDOW_MAXIMIZED)
			return WindowState::Maximised;

		return WindowState::Normal;
	}

	void WindowSDL2::Show()
	{
		SDL_ShowWindow( sdl_window );
	}

	void WindowSDL2::Hide()
	{
		SDL_HideWindow( sdl_window );
	}

	void WindowSDL2::SetVSync( const bool vsync )
	{
		SDL_GL_SetSwapInterval( vsync ? 1 : 0 );
	}

	void WindowSDL2::SwapBuffers()
	{
		SDL_GL_SwapWindow( sdl_window );
	}

	uint32_t WindowSDL2::GetSDLWindowId() const
	{
		return SDL_GetWindowID( sdl_window );
	}

	void WindowSDL2::HandleEvent( const SDL_WindowEvent& e )
	{
		switch (e.event)
		{
		case SDL_WINDOWEVENT_RESIZED:
			
			break;

		case SDL_WINDOWEVENT_MAXIMIZED:
			break;

		case SDL_WINDOWEVENT_MINIMIZED:
			break;

		case SDL_WINDOWEVENT_RESTORED:
			break;
		}
	}

	void* WindowSDL2::GetNativeHandle()
	{
#ifdef PLATFORM_WINDOWS
		SDL_SysWMinfo wminfo;
		SDL_VERSION( &wminfo.version );
		if (SDL_GetWindowWMInfo( sdl_window, &wminfo ) == 1)
			return wminfo.info.win.window;
#else
		return sdl_window;
#endif
		return nullptr;
	}

	std::string_view WindowSDL2::GetNativeHandleType() const
	{
#ifdef PLATFORM_WINDOWS
		return "HWND";
#elif defined(PLATFORM_APPLE)
		return "SDL";
#else
		return "";
#endif
	}
}
