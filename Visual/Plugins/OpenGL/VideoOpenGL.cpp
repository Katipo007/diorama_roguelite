#include "VideoOpenGL.hpp"

#include "Visual/Window.hpp"
#include "Visual/OpenGLContext.hpp"

#include "OpenGLHeader.hpp"
#include "TextureOpenGL.hpp"
#include "ShaderOpenGL.hpp"

namespace Graphics::API
{
	VideoOpenGL::VideoOpenGL( ::API::SystemAPI& system_ )
		: system( system_ )
	{
	}

	VideoOpenGL::~VideoOpenGL() = default;

	void VideoOpenGL::BeginRender()
	{
	}

	void VideoOpenGL::EndRender()
	{
		SwapFrameBuffers();
	}

	void VideoOpenGL::SetWindow( Graphics::WindowDefinition&& definition )
	{
		if (!initialised)
		{
#pragma push_macro("CreateWindow")
#undef CreateWindow
			window = system.CreateWindow( definition );
#pragma pop_macro("CreateWindow")
			InitOpenGL();
			window->Show();

			initialised = false;
		}
		// TODO: update window properties
	}

	const Graphics::Window& VideoOpenGL::GetWindow() const
	{
		ASSERT( window != nullptr );
		return *window;
	}

	bool VideoOpenGL::HasWindow() const
	{
		return window != nullptr;
	}

	void VideoOpenGL::SetVSync( const bool enabled_ )
	{
		if (vsync_enabled == enabled_)
			return;

		vsync_enabled = enabled_;
		if( window )
			window->SetVSync( vsync_enabled );
	}

	std::unique_ptr<Graphics::Texture> VideoOpenGL::CreateTexture( Size<uint16_t> size )
	{
		return std::make_unique<TextureOpenGL>( *this, size );
	}

	std::unique_ptr<Graphics::Shader> VideoOpenGL::CreateShader( const Graphics::ShaderDefinition& definition )
	{
		return std::make_unique<ShaderOpenGL>( definition );
	}

	std::string_view VideoOpenGL::GetShaderLanguage() const
	{
		return "glsl";
	}

	void VideoOpenGL::Init()
	{
	}

	void VideoOpenGL::Shutdown()
	{
		context.reset();
		system.DestroyWindow( window );
		window.reset();
	}

	void VideoOpenGL::InitOpenGL()
	{
		context = system.CreateOpenGLContext();
		context->Bind();

		//glewExperimental = GL_TRUE;
		const auto init_result = glewInit();
		if (init_result != GLEW_OK)
			throw std::runtime_error( "glew failed to initialise" ); // TODO: show error

#ifdef _DEBUG
		SetupDebugMessageCallback();
#endif

		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		glEnable( GL_DEPTH_TEST );
		glDepthFunc( GL_LEQUAL );

		ClearScreen();
	}

	void VideoOpenGL::SetupDebugMessageCallback()
	{
		glEnable( GL_DEBUG_OUTPUT );
		glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
		glDebugMessageCallback( []( unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char* message, const void* userParam )
			{
				reinterpret_cast<const VideoOpenGL*>(userParam)->OnOpenGLDebugMessage( source, type, id, severity, length, message );
			}, this );

		glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE );
	}

	void VideoOpenGL::ClearScreen()
	{
		glClearColor( 0, 0, 0, 1 );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		glDisable( GL_DEPTH_TEST );
		glDisable( GL_STENCIL_TEST );
		glDisable( GL_SCISSOR_TEST );
		glDisable( GL_CULL_FACE );

		// clear the back buffer as well
		SwapFrameBuffers();
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		SwapFrameBuffers();
	}

	void VideoOpenGL::SwapFrameBuffers()
	{
		if( window )
			window->Swap();
	}

	void VideoOpenGL::OnOpenGLDebugMessage( unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char* message ) const
	{
		(void)source;
		(void)type;
		(void)id;
		(void)length;

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         LOG_CRITICAL( OpenGL, message ); DEBUGBREAK(); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       LOG_ERROR( OpenGL, message ); return;
		case GL_DEBUG_SEVERITY_LOW:          LOG_WARN( OpenGL, message ); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_TRACE( OpenGL, message ); return;
		}

		ASSERT( false, "Unknown severity level!" );
	}
}
