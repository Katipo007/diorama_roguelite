#include "FramebufferOpenGL.hpp"
#include "OpenGLHeader.hpp"

namespace
{
	constexpr uint32_t MaxFramebufferSize = 8192u;
}

namespace Graphics::API
{
	FrameBufferOpenGL::FrameBufferOpenGL( const FrameBufferSpecification & spec )
		: specification( spec )
		, opengl_framebuffer_id( 0 )
		, opengl_colour_attachment( 0 )
		, opengl_depth_attachment( 0 )
	{
		Invalidate();
	}

	FrameBufferOpenGL::~FrameBufferOpenGL()
	{
		if( opengl_framebuffer_id )
			glDeleteFramebuffers( 1, &opengl_framebuffer_id );
		if( opengl_colour_attachment )
			glDeleteTextures( 1, &opengl_colour_attachment );
		if( opengl_depth_attachment )
			glDeleteTextures( 1, &opengl_depth_attachment );
	}

	void FrameBufferOpenGL::Invalidate()
	{
		// TODO: store currently bound frame buffer and restore after this operation. Juuuust to be safe

		if( opengl_framebuffer_id )
		{
			glDeleteFramebuffers( 1, &opengl_framebuffer_id );
			glDeleteTextures( 1, &opengl_colour_attachment );
			glDeleteTextures( 1, &opengl_depth_attachment );
		}

		glCreateFramebuffers( 1, &opengl_framebuffer_id );
		glBindFramebuffer( GL_FRAMEBUFFER, opengl_framebuffer_id );

		glCreateTextures( GL_TEXTURE_2D, 1, &opengl_colour_attachment );
		glBindTexture( GL_TEXTURE_2D, opengl_colour_attachment );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<GLsizei>( specification.size.width ), static_cast<GLsizei>( specification.size.height ), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, opengl_colour_attachment, 0 );

		glCreateTextures( GL_TEXTURE_2D, 1, &opengl_depth_attachment );
		glBindTexture( GL_TEXTURE_2D, opengl_depth_attachment );
		glTexStorage2D( GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, static_cast<GLsizei>( specification.size.width ), static_cast<GLsizei>( specification.size.height ) );
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, opengl_depth_attachment, 0 );

		ASSERT( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE, "Frame buffer is incomplete!" );

		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}

	void FrameBufferOpenGL::Bind()
	{
		glBindFramebuffer( GL_FRAMEBUFFER, opengl_framebuffer_id );
		glViewport( 0, 0, static_cast<GLsizei>( specification.size.width ), static_cast<GLsizei>( specification.size.height ) );
	}

	void FrameBufferOpenGL::Unbind()
	{
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}

	void FrameBufferOpenGL::Resize( uint32_t width, uint32_t height )
	{
		if( width == 0 || height == 0 )
		{
			LOG_WARN( LoggingChannels::OpenGL, "Attempting to resize framebuffer with a 0 dimension! ({0} x {1})", width, height );
			return;
		}
		else if( width > MaxFramebufferSize || height > MaxFramebufferSize)
		{
			LOG_WARN( LoggingChannels::OpenGL, "Attempting to resize framebuffer beyond limits! ({0} x {1})", width, height );
			return;
		}

		specification.size = { width, height };

		Invalidate();
	}
}
