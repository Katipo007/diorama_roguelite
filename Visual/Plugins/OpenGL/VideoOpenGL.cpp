#include "VideoOpenGL.hpp"

#include "Common/Core/API/SystemAPI.hpp"
#include "Visual/Window.hpp"
#include "Visual/OpenGLContext.hpp"

#include "OpenGLHeader.hpp"
#include "BufferOpenGL.hpp"
#include "FrameBufferOpenGL.hpp"
#include "ShaderOpenGL.hpp"
#include "TextureOpenGL.hpp"
#include "VertexArrayOpenGL.hpp"

namespace Graphics::API
{
	VideoOpenGL::VideoOpenGL( ::API::SystemAPI& system_ )
		: system( system_ )
	{
	}

	VideoOpenGL::~VideoOpenGL() = default;

	void VideoOpenGL::BeginRender()
	{
		// TODO: remove
		glClearColor( 0, 0, 0, 1 );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		glDisable( GL_DEPTH_TEST );
		glDisable( GL_STENCIL_TEST );
		glDisable( GL_SCISSOR_TEST );
		glDisable( GL_CULL_FACE );
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
#undef CreateWindow // we don't want the windows trash
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

		// Fetch capabilities
		{
			// max number of textures we can bind at once
			{
				int value = 0;
				glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &value );
				ASSERT( value >= 0 ); // hopefully never a negative number

				// hopefully they can support at least two textures
				if (value < 2)
					value = 2;

				capabilities.max_texture_slots = static_cast<uint32_t>(value);
			}

			// max 1d/2d texture resolution
			{
				int value = 0;
				glGetIntegerv( GL_MAX_TEXTURE_SIZE, &value );
				capabilities.max_texture_width = capabilities.max_texture_height = value;
			}

			// max cube-map resolution
			{
				int value = 0;
				glGetIntegerv( GL_MAX_CUBE_MAP_TEXTURE_SIZE, &value );
				capabilities.max_cubemap_width = capabilities.max_cubemap_height = value;
			}

			// max number of texture coordinates supported
			{
				int value = 0;
				glGetIntegerv( GL_MAX_TEXTURE_COORDS, &value );
				capabilities.max_texture_coordinates = value;
			}
		}

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
			window->SwapBuffers();
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

	void VideoOpenGL::SetViewport( Rect<uint32_t> rect )
	{
		glViewport( rect.x, rect.y, rect.GetWidth(), rect.GetHeight() );
	}

	Rect<uint32_t> VideoOpenGL::GetViewport() const
	{
		GLint viewport[4];
		glGetIntegerv( GL_VIEWPORT, viewport );
		
		return Rect( Point2D<uint32_t>( viewport[0], viewport[1] ), Size<uint32_t>( viewport[2], viewport[3] ) );
	}

	void VideoOpenGL::DrawIndexed( const std::shared_ptr<Graphics::VertexArray>& vertex_array, uint32_t index_count )
	{
		GLsizei count = index_count ? index_count : vertex_array->GetIndexBuffer()->GetCount();
		glDrawElements( GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL );
	}

	std::shared_ptr<Graphics::VertexBuffer> VideoOpenGL::CreateVertexBuffer( const Graphics::VertexBufferDefinition& definition ) const
	{
		return std::make_shared<VertexBufferOpenGL>( definition );
	}

	std::shared_ptr<Graphics::IndexBuffer> VideoOpenGL::CreateIndexBuffer( const Graphics::IndexBufferDefinition& definition ) const
	{
		return std::make_shared<IndexBufferOpenGL>( definition );
	}

	std::shared_ptr<Graphics::FrameBuffer> VideoOpenGL::CreateFrameBuffer( const Graphics::FrameBufferSpecification& specification ) const
	{
		return std::make_shared<FrameBufferOpenGL>( specification );
	}

	std::shared_ptr<Graphics::Shader> VideoOpenGL::CreateShader( const Filepath& filepath ) const
	{
		return std::make_shared<ShaderOpenGL>( filepath );
	}

	std::shared_ptr<Graphics::Shader> VideoOpenGL::CreateShader( std::string_view name, std::string_view vertex_src, std::string_view fragment_src ) const
	{
		return std::make_shared<ShaderOpenGL>( name, vertex_src, fragment_src );
	}

	std::shared_ptr<Graphics::Texture> VideoOpenGL::CreateTexture( const Graphics::TextureDefinition& definition ) const
	{
		return std::make_shared<TextureOpenGL>( definition );
	}

	std::shared_ptr<Graphics::Texture> VideoOpenGL::CreateTexture( const Filepath& filepath, const Graphics::TextureLoadProperties& props ) const
	{
		return std::make_shared<TextureOpenGL>( filepath, props );
	}

	std::shared_ptr<Graphics::VertexArray> VideoOpenGL::CreateVertexArray( const Graphics::VertexArrayDefinition& definition ) const
	{
		return std::make_shared<VertexArrayOpenGL>( definition );
	}

	std::string_view VideoOpenGL::GetName() const noexcept
	{
		return "OpenGL";
	}
}
