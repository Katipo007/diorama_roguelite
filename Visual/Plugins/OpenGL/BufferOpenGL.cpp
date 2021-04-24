#include "BufferOpenGL.hpp"
#include "OpenGLHeader.hpp"

#include <stack>

namespace
{
	std::stack<GLuint> BoundVboStack;

	void PushBoundVbo()
	{
		GLint current = 0;
		glGetIntegerv( GL_ARRAY_BUFFER_BINDING, &current );
		BoundVboStack.push( current );
	}

	void PopBoundVbo()
	{
		ASSERT( !BoundVboStack.empty() );
		glBindBuffer( GL_ARRAY_BUFFER, BoundVboStack.top() );
		BoundVboStack.pop();
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
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bound_ibo_stack.top() );
		bound_ibo_stack.pop();
	}
}

namespace Graphics::API
{
	VertexBufferOpenGL::VertexBufferOpenGL( const VertexBufferDefinition& definition )
		: name( definition.name.value_or( "Unnamed vertex buffer" ) )
		, layout( definition.layout )
	{
		PushBoundVbo();

		glCreateBuffers( 1, &vbo );
		Bind();
		glBufferData( GL_ARRAY_BUFFER, definition.data.size(), (void*)definition.data.data(), GL_DYNAMIC_DRAW );

		if (definition.name)
			glObjectLabel( GL_BUFFER, vbo, -1, definition.name.value().c_str() );

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

	IndexBufferOpenGL::IndexBufferOpenGL( const IndexBufferDefinition& definition )
		: name( definition.name.value_or( "Unnamed index buffer" ) )
		, count( (uint32_t)definition.indices.size() )
	{
		// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
		// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
		PushBoundVbo(); // ^ Vbo NOT Ibo
		glCreateBuffers( 1, &ibo );

		glBindBuffer( GL_ARRAY_BUFFER, ibo );
		glBufferData( GL_ARRAY_BUFFER, count * sizeof( uint32_t ), (void*)definition.indices.data(), GL_STATIC_DRAW );

		if (definition.name)
			glObjectLabel( GL_BUFFER, ibo, -1, definition.name.value().c_str() );

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
