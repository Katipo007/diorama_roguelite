#include "WindowsWindow.hpp"

#ifdef PLATFORM_WINDOWS

#include "Visual/Input/Input.hpp"
#include "Visual/Events/ApplicationEvents.hpp"
#include "Visual/Events/KeyboardEvents.hpp"
#include "Visual/Events/MouseEvents.hpp"

#include "Visual/Device/RendererCommand.hpp"
#include "Visual/Platform/OpenGL/RendererAPIOpenGL.hpp"
#include "Visual/Platform/OpenGL/GraphicsContext/SDL2.hpp"

#include <SDL2/SDL.h>
#ifdef _WIN64
#	pragma comment(lib, "Visual/Vendor/SDL2/lib/x64/SDL2.lib")
#else
#	pragma comment(lib, "Visual/Vendor/SDL2/lib/x86/SDL2.lib")
#endif

#ifdef DEARIMGUI_ENABLED
#include "Visual/DearImGui/DearImGui.hpp"
#pragma warning(push, 0)
#include "Visual/Vendor/dearimgui/backends/imgui_impl_sdl.h"
#pragma warning(pop)
#endif

namespace
{
	// We keep track of the number of SDL windows currently active so we can shut-down SDL
	// once all windows are closed
	uint8_t s_sdl_window_count = 0;

	void InitSDL()
	{
		if (SDL_Init( SDL_INIT_VIDEO ) != 0)
		{
			LOG_CRITICAL( Application, "SDL2 could not initialize! SDL_Error: '{0}'", SDL_GetError() );
			FATAL( "SDL2 could not initialize!" );
		}
	}

	void ShutdownSDL()
	{
		ASSERT( s_sdl_window_count == 0 );
		SDL_Quit();
	}
}

namespace Visual::Device
{
	WindowsWindow::WindowsWindow( const WindowCreationProperties properties )
		: window( NULL )
		, renderer_api( nullptr )
		, graphics_context( nullptr )
	{
		if (s_sdl_window_count == 0)
			InitSDL();

		// initialise the SDL2 window
		{
			auto window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

			if (properties.fullscreen)
				window_flags |= SDL_WINDOW_MAXIMIZED;

			switch (properties.renderer_api)
			{
#ifdef RENDERER_IMPLEMENTATION_NONE
			case RendererAPIType::API::None:
				// intentionally blank
				break;
#endif

#ifdef RENDERER_IMPLEMENTATION_OPENGL
			case RendererAPIs::API::OpenGL:
				SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
				SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 5 );
				SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ); // enable double buffering
				SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 ); // 24bit z buffer, may need to turn down depending on the system

				window_flags |= SDL_WINDOW_OPENGL;
				break;
#endif

			default:
				LOG_CRITICAL( Client, "Unsupported renderer type" );
				break;
			}

			window = SDL_CreateWindow( properties.title.c_str(),
				SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				static_cast<int>(properties.width), static_cast<int>(properties.height),
				window_flags );

			if (!window)
			{
				LOG_CRITICAL( Application, "Application window could not be created! SDL_Error: '{0}'", SDL_GetError() );
				FATAL( "Application window could not be created!" );
			}
		}

		// initialise the graphics context object
		{
			switch (properties.renderer_api)
			{
#ifdef RENDERER_IMPLEMENTATION_NONE
			case RendererAPIType::API::None:
				// intentionally blank
				break;
#endif

#ifdef RENDERER_IMPLEMENTATION_OPENGL
			case RendererAPIs::API::OpenGL:
				graphics_context = std::make_unique< OpenGL::GraphicsContextOpenGL_SDL2 >( window );
				renderer_api = std::make_unique< OpenGL::RendererAPIOpenGL >();
				break;
#endif

			default:
				LOG_CRITICAL( Client, "Unsupported renderer type" );
				break;
			}

			ASSERT( graphics_context != nullptr );
			ASSERT( renderer_api != nullptr );

			graphics_context->Init();
			renderer_api->Init();
		}

		++s_sdl_window_count;
	}

	WindowsWindow::~WindowsWindow()
	{
		if (graphics_context)
		{
			graphics_context->Shutdown();
			graphics_context.reset();
		}

		if (renderer_api)
		{
			renderer_api->Shutdown();
			renderer_api.reset();
		}

		SDL_DestroyWindow( window );
		window = NULL;
		--s_sdl_window_count;

		if (s_sdl_window_count == 0)
			ShutdownSDL();
	}

	uint32_t WindowsWindow::GetWidth() const
	{
		int w = 0;
		SDL_GetWindowSize( window, &w, NULL );

		return static_cast<uint32_t>(w);
	}

	uint32_t WindowsWindow::GetHeight() const
	{
		int h = 0;
		SDL_GetWindowSize( window, NULL, &h );

		return static_cast<uint32_t>(h);
	}

	double WindowsWindow::GetApplicationTime() const
	{
		return SDL_GetTicks() / 1000.0;
	}

	void WindowsWindow::SetEventCallback( const EventCallbackFunc_t& callback )
	{
		event_callback = callback;
	}

	void* WindowsWindow::GetNativeWindow() const
	{
		return window;
	}

	RendererAPIs::API WindowsWindow::GetRendererAPIType() const
	{
		return renderer_api->GetAPIType();
	}

	RendererAPI& WindowsWindow::GetRendererAPI() const
	{
		return *renderer_api;
	}

	void WindowsWindow::OnUpdateBegin()
	{
		SDL_Event currentEvent;
		while (SDL_PollEvent( &currentEvent ) != 0)
		{
#ifdef DEARIMGUI_ENABLED
			if (DearImGui::IsEnabled())
				ImGui_ImplSDL2_ProcessEvent( &currentEvent );
#endif

			switch (currentEvent.type)
			{
			case SDL_QUIT:
			{
				if (event_callback)
				{
					ApplicationEvents::WindowClose e( *this );
					event_callback( e );
				}
			}
			break;

			case SDL_EventType::SDL_WINDOWEVENT:
			{
				if (currentEvent.window.event == SDL_WindowEventID::SDL_WINDOWEVENT_RESIZED)
				{
					if (event_callback)
					{
						renderer_api->SetViewport( 0, 0, currentEvent.window.data1, currentEvent.window.data2 );
						ApplicationEvents::WindowResized e( *this, static_cast<uint16_t>(currentEvent.window.data1), static_cast<uint16_t>(currentEvent.window.data2) );
						event_callback( e );
					}
				}
			}
			break;

#pragma region Keyboard events
			case SDL_KEYDOWN: /**< Key pressed */
			{
				if (event_callback)
				{
					KeyboardEvents::KeyPressed e( currentEvent.key.keysym.sym, static_cast<uint16_t>(currentEvent.key.repeat > 0) ); // TODO: repeat count
					event_callback( e );
				}
			}
			break;

			case SDL_KEYUP: /**< Key released */
			{
				if (event_callback)
				{
					KeyboardEvents::KeyReleased e( currentEvent.key.keysym.sym );
					event_callback( e );
				}
			}
			break;

			case SDL_TEXTEDITING: /**< Keyboard text editing (composition) */
				// TODO
				break;

			case SDL_TEXTINPUT: /**< Keyboard text input */
				// TODO
				break;

			case SDL_KEYMAPCHANGED: /**< Key-map changed due to a system event such as an input language or keyboard layout change. */
				// TODO
				break;
#pragma endregion

#pragma region Mouse events
			case SDL_MOUSEMOTION: /**< Mouse moved */
			{
				if (event_callback)
				{
					MouseEvents::Moved e( (float)currentEvent.motion.x, (float)currentEvent.motion.y );
					event_callback( e );
				}
			}
			break;

			case SDL_MOUSEBUTTONDOWN: /**< Mouse button pressed */
			{
				if (event_callback)
				{
					MouseEvents::ButtonPressed e( currentEvent.button.button ); // TODO: use number of clicks?
					event_callback( e );
				}
			}
			break;

			case SDL_MOUSEBUTTONUP: /**< Mouse button released */
			{
				if (event_callback)
				{
					MouseEvents::ButtonReleased e( currentEvent.button.button );
					event_callback( e );
				}
			}
			break;

			case SDL_MOUSEWHEEL: /**< Mouse wheel motion */
			{
				if (event_callback)
				{
					MouseEvents::Scrolled e( (float)currentEvent.wheel.x, (float)currentEvent.wheel.y );
					event_callback( e );
				}
			}
			break;
#pragma endregion

#pragma region Joystick events
			case SDL_JOYAXISMOTION: /**< Joystick axis motion */
				// TODO
				break;

			case SDL_JOYBALLMOTION: /**< Joystick trackball motion */
				// TODO
				break;

			case SDL_JOYHATMOTION: /**< Joystick hat position change */
				// TODO
				break;

			case SDL_JOYBUTTONDOWN: /**< Joystick button pressed */
				// TODO
				break;

			case SDL_JOYBUTTONUP: /**< Joystick button released */
				// TODO
				break;

			case SDL_JOYDEVICEADDED: /**< A new joystick has been inserted into the system */
				// TODO
				break;

			case SDL_JOYDEVICEREMOVED: /**< An opened joystick has been removed */
				// TODO
				break;
#pragma endregion

#pragma region Game controller events
			case SDL_CONTROLLERAXISMOTION: /**< Game controller axis motion */
				// TODO
				break;

			case SDL_CONTROLLERBUTTONDOWN: /**< Game controller button pressed */
				// TODO
				break;

			case SDL_CONTROLLERBUTTONUP: /**< Game controller button released */
				// TODO
				break;

			case SDL_CONTROLLERDEVICEADDED: /**< A new Game controller has been inserted into the system */
				// TODO
				break;

			case SDL_CONTROLLERDEVICEREMOVED: /**< An opened Game controller has been removed */
				// TODO
				break;

			case SDL_CONTROLLERDEVICEREMAPPED: /**< The controller mapping was updated */
				// TODO
				break;
#pragma endregion

			case SDL_AUDIODEVICEADDED: /**< A new audio device is available */
				// TODO
				break;

			case SDL_AUDIODEVICEREMOVED: /**< An audio device has been removed. */
				// TODO
				break;

			case SDL_RENDER_TARGETS_RESET: /**< The render targets have been reset and their contents need to be updated */
				// TODO
				break;

			case SDL_RENDER_DEVICE_RESET: /**< The device has been reset and all textures need to be recreated */
				// TODO
				break;
			}
		}

		graphics_context->FrameBegin();
	}

	void WindowsWindow::OnUpdateEnd()
	{
		graphics_context->FrameEnd();
		graphics_context->SwapBuffers();
	}
}

#endif // PLATFORM_WINDOWS
