#include "RendererAPIOpenGL.hpp"

#ifdef RENDERER_IMPLEMENTATION_OPENGL
#include "OpenGLHeader.hpp"

#include "BufferOpenGL.hpp"
#include "FramebufferOpenGL.hpp"
#include "ShaderOpenGL.hpp"
#include "TextureOpenGL.hpp"
#include "VertexArrayOpenGL.hpp"

namespace Visual::Device::OpenGL
{
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam )
	{
		(void)source;
		(void)type;
		(void)id;
		(void)length;
		(void)userParam;

		switch( severity )
		{
			case GL_DEBUG_SEVERITY_HIGH:         LOG_CRITICAL( OpenGL, message ); DEBUGBREAK(); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       LOG_ERROR( OpenGL, message ); return;
			case GL_DEBUG_SEVERITY_LOW:          LOG_WARN( OpenGL, message ); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_TRACE( OpenGL, message ); return;
		}

		ASSERT( false, "Unknown severity level!" );
	}

	void RendererAPIOpenGL::Init()
	{
		//glewExperimental = GL_TRUE;
		const auto init_result = glewInit();
		ASSERT( init_result == GLEW_OK );

#ifdef _DEBUG
		glEnable( GL_DEBUG_OUTPUT );
		glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
		glDebugMessageCallback( OpenGLMessageCallback, nullptr );

		glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE );
#endif

		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		glEnable( GL_DEPTH_TEST );
		glDepthFunc( GL_LEQUAL );


		// TEMP
		glDisable( GL_CULL_FACE );

		// fetch and store capabilities
		{
			// clear
			memset( &cached_capabilities, 0, sizeof( cached_capabilities ) );

			{
				

			}
		}
	}

	void RendererAPIOpenGL::Shutdown()
	{

	}

	void RendererAPIOpenGL::SetViewport( uint32_t x, uint32_t y, uint32_t w, uint32_t h )
	{
		glViewport( x, y, w, h );
	}

	uint32_t RendererAPIOpenGL::GetViewportX()
	{
		GLint viewport[ 4 ];
		glGetIntegerv( GL_VIEWPORT, viewport );
		return viewport[ 0 ];
	}

	uint32_t RendererAPIOpenGL::GetViewportY()
	{
		GLint viewport[ 4 ];
		glGetIntegerv( GL_VIEWPORT, viewport );
		return viewport[ 1 ];
	}

	uint32_t RendererAPIOpenGL::GetViewportWidth()
	{
		GLint viewport[ 4 ];
		glGetIntegerv( GL_VIEWPORT, viewport );
		return viewport[ 2 ];
	}

	uint32_t RendererAPIOpenGL::GetViewportHeight()
	{
		GLint viewport[ 4 ];
		glGetIntegerv( GL_VIEWPORT, viewport );
		return viewport[ 3 ];
	}

	void RendererAPIOpenGL::SetClearColour( ColourRGBA colour )
	{
		const auto colour_floats = colour.AsFloatsRGBA();
		glClearColor( colour_floats.r, colour_floats.g, colour_floats.b, colour_floats.a );
	}

	void RendererAPIOpenGL::Clear()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	}

	void RendererAPIOpenGL::DrawIndexed( const std::shared_ptr<VertexArray>& vertex_array, uint32_t index_count )
	{
		GLsizei count = index_count ? index_count : vertex_array->GetIndexBuffer()->GetCount();
		glDrawElements( GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL );
	}

	const RendererCapabilities& RendererAPIOpenGL::GetCapabilities() const
	{
		return cached_capabilities;
	}

	std::shared_ptr<VertexBuffer> RendererAPIOpenGL::CreateVertexBuffer( const VertexBuffer::CreationProperties& props ) const
	{
		return std::make_shared<VertexBufferOpenGL>( props );
	}

	std::shared_ptr<IndexBuffer> RendererAPIOpenGL::CreateIndexBuffer( const IndexBuffer::CreationProperties& props ) const
	{
		return std::make_shared<IndexBufferOpenGL>( props );
	}

	std::shared_ptr<FrameBuffer> RendererAPIOpenGL::CreateFrameBuffer( const FrameBuffer::Specification & spec ) const
	{
		return std::make_shared<FrameBufferOpenGL>( spec );
	}

	std::shared_ptr<Shader> RendererAPIOpenGL::CreateShader( const std::filesystem::path& filepath ) const
	{
		return std::make_shared<ShaderOpenGL>( filepath );
	}

	std::shared_ptr<Shader> RendererAPIOpenGL::CreateShader( std::string_view name, std::string_view vertex_src, std::string_view fragment_src ) const
	{
		return std::make_shared<ShaderOpenGL>( name, vertex_src, fragment_src );
	}

	std::shared_ptr<Texture2D> RendererAPIOpenGL::CreateTexture2D( uint32_t width, uint32_t height, const Texture::CreationProperties& props ) const
	{
		return std::make_shared<Texture2DOpenGL>( width, height, props );
	}

	std::shared_ptr<Texture2D> RendererAPIOpenGL::CreateTexture2D( const std::filesystem::path& filepath, const Texture::LoadProperties& props ) const
	{
		return std::make_shared<Texture2DOpenGL>( filepath, props );
	}

	std::shared_ptr<VertexArray> RendererAPIOpenGL::CreateVertexArray( const VertexArray::CreationProperties& props ) const
	{
		return std::make_shared<VertexArrayOpenGL>( props );
	}
}

#endif
