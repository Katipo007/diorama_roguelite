#include "Precompiled/pch.hpp"
#include "SDL2.hpp"

#ifdef RENDERER_IMPLEMENTATION_OPENGL

#include "Visual/Platform/OpenGL/OpenGLHeader.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#ifdef DEARIMGUI_ENABLED
#	include "DearImGui/DearImGui.hpp"
#	define IMGUI_IMPL_OPENGL_LOADER_GLEW
#	include "Visual/Vendor/dearimgui/backends/imgui_impl_opengl3.h"
#	include "Visual/Vendor/dearimgui/backends/imgui_impl_sdl.h"
#endif

// TODO: move this file to Platform/Windows since all these calls are specific to SDL2

namespace Visual::Device::OpenGL
{
	GraphicsContextOpenGL_SDL2::GraphicsContextOpenGL_SDL2( SDL_Window* window_handle )
		: window_handle( window_handle )
		, gl_context( NULL )
	{
		ASSERT( window_handle != NULL );
	}

	void GraphicsContextOpenGL_SDL2::Init()
	{
		ASSERT( gl_context == NULL );

		gl_context = SDL_GL_CreateContext( window_handle );

		LOG_INFO( "OpenGL Info:" );
		LOG_INFO( "\tVendor: {0}", glGetString( GL_VENDOR ) );
		LOG_INFO( "\tRenderer: {0}", glGetString( GL_RENDERER ) );
		LOG_INFO( "\tVersion: {0}", glGetString( GL_VERSION ) );

#ifdef DEARIMGUI_ENABLED
		ImGui_ImplSDL2_InitForOpenGL( window_handle, NULL );
		ImGui_ImplOpenGL3_Init( "#version 410" );
#endif
	}

	void GraphicsContextOpenGL_SDL2::Shutdown()
	{
		ASSERT( gl_context != NULL );

#ifdef DEARIMGUI_ENABLED
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
#endif

		SDL_GL_DeleteContext( gl_context );
		gl_context = NULL;
	}

	void GraphicsContextOpenGL_SDL2::FrameBegin()
	{
#ifdef DEARIMGUI_ENABLED
		if( DearImGui::IsEnabled() )
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame( window_handle );
		}
#endif
	}

	void GraphicsContextOpenGL_SDL2::FrameEnd()
	{
#ifdef DEARIMGUI_ENABLED
		if( DearImGui::IsEnabled() )
			ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
#endif
	}

	void GraphicsContextOpenGL_SDL2::SwapBuffers()
	{
		ASSERT( gl_context != NULL );

		SDL_GL_SwapWindow( window_handle );
	}
}

// ImGui implementation
#ifdef DEARIMGUI_ENABLED
#	pragma warning(push, 0)

#	if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#		define _CRT_SECURE_NO_WARNINGS
#		pragma warning(disable : 4996)
#	endif

#	include "Visual/Vendor/dearimgui/backends/imgui_impl_opengl3.cpp"
#	include "Visual/Vendor/dearimgui/backends/imgui_impl_sdl.cpp"

#	pragma warning(pop)
#endif // DEARIMGUI_ENABLED

#endif
