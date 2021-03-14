#include "Precompiled/pch.hpp"
#include "BufferOpenGL.hpp"

#ifdef RENDERER_IMPLEMENTATION_OPENGL

#include "OpenGLHeader.hpp"

#include <stack>

namespace
{
	std::stack<GLuint> bound_vbo_stack;

	void PushBoundVbo()
	{
		GLint current = 0;
		glGetIntegerv( GL_ARRAY_BUFFER_BINDING, &current );
		bound_vbo_stack.push( current );
	}

	void PopBoundVbo()
	{
		ASSERT( !bound_vbo_stack.empty() );
		glBindVertexArray( bound_vbo_stack.top() );
		bound_vbo_stack.pop();
	}


	std::stack<GLuint> bound_ibo_stack;

	void PushBoundIbo()
	{
		GLint current = 0;
		glGetIntegerv( GL_ELEMENT_ARRAY_BUFFER_BINDING, &current );
		bound_ibo_stack.push( current );
	}

	void PopBoundIbo()
	{
		ASSERT( !bound_ibo_stack.empty() );
		glBindVertexArray( bound_ibo_stack.top() );
		bound_ibo_stack.pop();
	}
}

namespace Visual::Device::OpenGL
{
	VertexBufferOpenGL::VertexBufferOpenGL( const CreationProperties& props )
		: name( props.name )
		, layout( props.layout )
	{
		PushBoundVbo();

		glCreateBuffers( 1, &vbo );
		Bind();
		glBufferData( GL_ARRAY_BUFFER, props.data.size(), (void*)props.data.data(), GL_DYNAMIC_DRAW );

		if (!props.name.empty())
			glObjectLabel( GL_BUFFER, vbo, -1, props.name.c_str() );

		Unbind();
		PopBoundVbo();
	}

	VertexBufferOpenGL::~VertexBufferOpenGL()
	{
		glDeleteBuffers( 1, &vbo );
	}

	void VertexBufferOpenGL::Bind() const
	{
		glBindBuffer( GL_ARRAY_BUFFER, vbo );
	}

	void VertexBufferOpenGL::Unbind() const
	{
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
	}

	void VertexBufferOpenGL::SetData( const void* data, uint32_t size )
	{
		PushBoundVbo();

		Bind();
		glBufferSubData( GL_ARRAY_BUFFER, 0, size, data );
		Unbind();

		PopBoundVbo();
	}

	void VertexBufferOpenGL::SetLayout( const BufferLayout & layout_ )
	{
		layout = layout_;
	}

	IndexBufferOpenGL::IndexBufferOpenGL( const CreationProperties& props )
		: name( props.name )
		, count( (uint32_t)props.indices.size() )
	{
		// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
		// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
		PushBoundVbo(); // ^ Vbo NOT Ibo
		glCreateBuffers( 1, &ibo );

		glBindBuffer( GL_ARRAY_BUFFER, ibo );
		glBufferData( GL_ARRAY_BUFFER, count * sizeof( uint32_t ), (void*)props.indices.data(), GL_STATIC_DRAW );

		PopBoundVbo(); // Vbo NOT Ibo to match above
	}

	IndexBufferOpenGL::~IndexBufferOpenGL()
	{
		glDeleteBuffers( 1, &ibo );
	}

	void IndexBufferOpenGL::Bind() const
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
	}

	void IndexBufferOpenGL::Unbind() const
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}
}

#endif
