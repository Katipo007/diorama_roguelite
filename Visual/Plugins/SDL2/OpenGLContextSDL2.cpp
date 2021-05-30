#include "OpenGLContextSDL2.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

namespace Graphics::API
{
	OpenGLContextSDL2::OpenGLContextSDL2( SDL_Window* window_ )
		: window( window_ )
		, context_owner( true )
	{
		ASSERT( window != nullptr );
		SDL_GL_SetAttribute( SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1 );
		shared_context = SDL_GL_CreateContext( window );
		context = SDL_GL_CreateContext( window );

		LOG_INFO( LoggingChannels::OpenGL, "OpenGL Info:" );
		LOG_INFO( LoggingChannels::OpenGL, "\tVendor: {0}", glGetString( GL_VENDOR ) );
		LOG_INFO( LoggingChannels::OpenGL, "\tRenderer: {0}", glGetString( GL_RENDERER ) );
		LOG_INFO( LoggingChannels::OpenGL, "\tVersion: {0}", glGetString( GL_VERSION ) );
	}

	OpenGLContextSDL2::OpenGLContextSDL2( SDL_Window* window_, SDL_GLContext shared_ )
		: window( window_ )
		, context( shared_ )
		, shared_context( nullptr )
		, context_owner( false )
	{

	}
	
	OpenGLContextSDL2::~OpenGLContextSDL2()
	{
		SDL_GL_MakeCurrent( window, nullptr );
		if (context_owner)
		{
			SDL_GL_DeleteContext( shared_context );
			SDL_GL_DeleteContext( context );
		}
	}

	void OpenGLContextSDL2::Bind()
	{
		SDL_GL_MakeCurrent( window, context );
	}
	
	std::unique_ptr<OpenGLContext> OpenGLContextSDL2::CreateSharedContext()
	{
		return std::make_unique<OpenGLContextSDL2>( window, shared_context );
	}
}
