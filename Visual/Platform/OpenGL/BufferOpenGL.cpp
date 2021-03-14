#include "Precompiled/pch.hpp"
#include "BufferOpenGL.hpp"

#ifdef RENDERER_IMPLEMENTATION_OPENGL

#include "OpenGLHeader.hpp"

namespace Visual::Device::OpenGL
{
	VertexBufferOpenGL::VertexBufferOpenGL( uint32_t size )
	{
		glCreateBuffers( 1, &opengl_vb_id );
		glBindBuffer( GL_ARRAY_BUFFER, opengl_vb_id );
		glBufferData( GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW );
	}

	VertexBufferOpenGL::VertexBufferOpenGL( float* vertices, uint32_t size )
	{
		glCreateBuffers( 1, &opengl_vb_id );
		glBindBuffer( GL_ARRAY_BUFFER, opengl_vb_id );
		glBufferData( GL_ARRAY_BUFFER, size, (void*)vertices, GL_DYNAMIC_DRAW );
	}

	VertexBufferOpenGL::~VertexBufferOpenGL()
	{
		glDeleteBuffers( 1, &opengl_vb_id );
	}

	void VertexBufferOpenGL::Bind() const
	{
		glBindBuffer( GL_ARRAY_BUFFER, opengl_vb_id );
	}

	void VertexBufferOpenGL::Unbind() const
	{
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
	}

	void VertexBufferOpenGL::SetData( const void* data, uint32_t size )
	{
		glBindBuffer( GL_ARRAY_BUFFER, opengl_vb_id );
		glBufferSubData( GL_ARRAY_BUFFER, 0, size, data );
	}

	void VertexBufferOpenGL::SetLayout( const BufferLayout & layout_ )
	{
		layout = layout_;
	}

	IndexBufferOpenGL::IndexBufferOpenGL( uint32_t* indices_, uint32_t count_ )
		: count( count_ )
	{
		glCreateBuffers( 1, &opengl_ib_id );

		// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
		// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
		glBindBuffer( GL_ARRAY_BUFFER, opengl_ib_id );
		glBufferData( GL_ARRAY_BUFFER, count * sizeof( uint32_t ), (void*)indices_, GL_STATIC_DRAW );
	}

	IndexBufferOpenGL::~IndexBufferOpenGL()
	{
		glDeleteBuffers( 1, &opengl_ib_id );
	}

	void IndexBufferOpenGL::Bind() const
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, opengl_ib_id );
	}

	void IndexBufferOpenGL::Unbind() const
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}
}

#endif
