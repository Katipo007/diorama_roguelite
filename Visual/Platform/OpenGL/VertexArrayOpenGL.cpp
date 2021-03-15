#include "Precompiled/pch.hpp"
#include "VertexArrayOpenGL.hpp"

#ifdef RENDERER_IMPLEMENTATION_OPENGL

#include <stack>

namespace
{
	static GLenum GetShaderDataTypeToOpenGLBaseType( ::Visual::Device::ShaderDataType type )
	{
		switch( type )
		{
			case ::Visual::Device::ShaderDataType::Float:
			case ::Visual::Device::ShaderDataType::Float2:
			case ::Visual::Device::ShaderDataType::Float3:
			case ::Visual::Device::ShaderDataType::Float4:
			case ::Visual::Device::ShaderDataType::Mat3:
			case ::Visual::Device::ShaderDataType::Mat4:
				return GL_FLOAT;

			case ::Visual::Device::ShaderDataType::Int:
			case ::Visual::Device::ShaderDataType::Int2:
			case ::Visual::Device::ShaderDataType::Int3:
			case ::Visual::Device::ShaderDataType::Int4:
				return GL_INT;

			case ::Visual::Device::ShaderDataType::uInt:
			case ::Visual::Device::ShaderDataType::uInt2:
			case ::Visual::Device::ShaderDataType::uInt3:
			case ::Visual::Device::ShaderDataType::uInt4:
				return GL_UNSIGNED_INT;

			case ::Visual::Device::ShaderDataType::Bool:
				return GL_BOOL;
		}

		ASSERT( false, "Unrecognised ShaderDataType!" );
		return 0;
	}

	std::stack<GLuint> bound_vao_stack;

	void PushBoundVao()
	{
		GLint current = 0;
		glGetIntegerv( GL_VERTEX_ARRAY_BINDING, &current );
		bound_vao_stack.push( current );
	}

	void PopBoundVao()
	{
		ASSERT( !bound_vao_stack.empty() );
		glBindVertexArray( bound_vao_stack.top() );
		bound_vao_stack.pop();
	}

	GLuint GetBoundVao()
	{
		GLint current = 0;
		glGetIntegerv( GL_VERTEX_ARRAY_BINDING, &current );
		return current;
	}
}

namespace Visual::Device::OpenGL
{
	VertexArrayOpenGL::VertexArrayOpenGL( const CreationProperties& props )
		: name( props.name )
		, vao( 0 )
		, vbi( 0 )
	{
		PushBoundVao();

		glCreateVertexArrays( 1, &vao );
		Bind();

		if (!props.name.empty())
			glObjectLabel( GL_VERTEX_ARRAY, vao, -1, props.name.c_str() );

		for (auto& vb : props.vertex_buffers)
			AddVertexBuffer( vb );

		if (props.index_buffer != nullptr)
			SetIndexBuffer( props.index_buffer );

		Unbind();
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

		PopBoundVao();
	}

	VertexArrayOpenGL::~VertexArrayOpenGL()
	{
		glDeleteVertexArrays( 1, &vao );
	}

	void VertexArrayOpenGL::Bind() const
	{
		glBindVertexArray( vao );
	}

	void VertexArrayOpenGL::Unbind() const
	{
		glBindVertexArray( 0 );
	}

	void VertexArrayOpenGL::AddVertexBuffer( const std::shared_ptr<VertexBuffer>& vertex_buffer )
	{
		ASSERT( GetBoundVao() == vao );
		ASSERT( vertex_buffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!" );

		vertex_buffer->Bind();
		vertex_buffers.push_back( vertex_buffer );

		const auto& layout = vertex_buffer->GetLayout();
		for( const auto& element : layout )
		{
			switch( element.type )
			{
				case ::Visual::Device::ShaderDataType::Float:
				case ::Visual::Device::ShaderDataType::Float2:
				case ::Visual::Device::ShaderDataType::Float3:
				case ::Visual::Device::ShaderDataType::Float4:
				{
					glEnableVertexAttribArray( vbi );
					glVertexAttribPointer( vbi
										   , static_cast<GLint>( element.GetComponentCount() )
										   , GetShaderDataTypeToOpenGLBaseType( element.type )
										   , element.normalised ? GL_TRUE : GL_FALSE
										   , static_cast<GLsizei>( layout.GetStride() )
										   , (const void*)element.offset
					);
					++vbi;
					break;
				}

				case ::Visual::Device::ShaderDataType::Int:
				case ::Visual::Device::ShaderDataType::Int2:
				case ::Visual::Device::ShaderDataType::Int3:
				case ::Visual::Device::ShaderDataType::Int4:
				case ::Visual::Device::ShaderDataType::uInt:
				case ::Visual::Device::ShaderDataType::uInt2:
				case ::Visual::Device::ShaderDataType::uInt3:
				case ::Visual::Device::ShaderDataType::uInt4:
				case ::Visual::Device::ShaderDataType::Bool:
				{
					glEnableVertexAttribArray( vbi );
					glVertexAttribIPointer( vbi
						, static_cast<GLint>(element.GetComponentCount())
						, GetShaderDataTypeToOpenGLBaseType( element.type )
						, static_cast<GLsizei>(layout.GetStride())
						, (const void*)element.offset
					);
					++vbi;
					break;
				}

				case ::Visual::Device::ShaderDataType::Mat3:
				case ::Visual::Device::ShaderDataType::Mat4:
				{
					const auto count = element.GetComponentCount();
					for( uint32_t i = 0; i < count; ++i )
					{
						glEnableVertexAttribArray( vao );
						glVertexAttribPointer( vbi
											   , static_cast<GLint>( count )
											   , GetShaderDataTypeToOpenGLBaseType( element.type )
											   , element.normalised ? GL_TRUE : GL_FALSE
											   , static_cast<GLsizei>( layout.GetStride() )
											   , (const void*)( element.offset + sizeof( float ) * count * i )
						);
						glVertexAttribDivisor( vbi, 1 );
						++vbi;
					}
					break;
				}

				default:
					ASSERT( false, "Unrecognised ShaderDataType!" );
					break;
			}
		}
	}

	void VertexArrayOpenGL::SetIndexBuffer( const std::shared_ptr<IndexBuffer>& new_index_buffer )
	{
		ASSERT( GetBoundVao() == vao );
		ASSERT( !this->index_buffer );

		new_index_buffer->Bind();
		this->index_buffer = new_index_buffer;
	}

	const std::shared_ptr<VertexBuffer>& VertexArrayOpenGL::GetVertexBuffer( size_t i ) const
	{
		ASSERT( i < vertex_buffers.size() );
		return vertex_buffers[ i ];
	}

	size_t VertexArrayOpenGL::GetVertexBufferCount() const
	{
		return vertex_buffers.size();
	}
}
#endif
