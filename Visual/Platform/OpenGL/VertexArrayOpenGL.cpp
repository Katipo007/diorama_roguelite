#include "Precompiled/pch.hpp"
#include "VertexArrayOpenGL.hpp"

#ifdef RENDERER_IMPLEMENTATION_OPENGL

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

			case ::Visual::Device::ShaderDataType::Bool:
				return GL_BOOL;
		}

		ASSERT( false, "Unrecognised ShaderDataType!" );
		return 0;
	}
}

namespace Visual::Device::OpenGL
{
	VertexArrayOpenGL::VertexArrayOpenGL()
		: opengl_vertexarray_id( 0 )
		, opengl_vertexbuffer_index( 0 )
	{
		glCreateVertexArrays( 1, &opengl_vertexarray_id );
	}

	VertexArrayOpenGL::~VertexArrayOpenGL()
	{
		glDeleteVertexArrays( 1, &opengl_vertexarray_id );
	}

	void VertexArrayOpenGL::Bind() const
	{
		glBindVertexArray( opengl_vertexarray_id );
	}

	void VertexArrayOpenGL::Unbind() const
	{
		glBindVertexArray( 0 );
	}

	void VertexArrayOpenGL::AddVertexBuffer( const std::shared_ptr<VertexBuffer>& vertex_buffer )
	{
		ASSERT( vertex_buffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!" );

		glBindVertexArray( opengl_vertexarray_id );
		vertex_buffer->Bind();

		const auto& layout = vertex_buffer->GetLayout();
		for( const auto& element : layout )
		{
			switch( element.type )
			{
				case ::Visual::Device::ShaderDataType::Float:
				case ::Visual::Device::ShaderDataType::Float2:
				case ::Visual::Device::ShaderDataType::Float3:
				case ::Visual::Device::ShaderDataType::Float4:
				case ::Visual::Device::ShaderDataType::Int:
				case ::Visual::Device::ShaderDataType::Int2:
				case ::Visual::Device::ShaderDataType::Int3:
				case ::Visual::Device::ShaderDataType::Int4:
				case ::Visual::Device::ShaderDataType::Bool:
				{
					glEnableVertexAttribArray( opengl_vertexarray_id );
					glVertexAttribPointer( opengl_vertexbuffer_index
										   , static_cast<GLint>( element.GetComponentCount() )
										   , GetShaderDataTypeToOpenGLBaseType( element.type )
										   , element.normalised ? GL_TRUE : GL_FALSE
										   , static_cast<GLsizei>( layout.GetStride() )
										   , (const void*)element.offset
					);
					++opengl_vertexbuffer_index;
					break;
				}

				case ::Visual::Device::ShaderDataType::Mat3:
				case ::Visual::Device::ShaderDataType::Mat4:
				{
					const auto count = element.GetComponentCount();
					for( uint32_t i = 0; i < count; ++i )
					{
						glEnableVertexAttribArray( opengl_vertexarray_id );
						glVertexAttribPointer( opengl_vertexbuffer_index
											   , static_cast<GLint>( count )
											   , GetShaderDataTypeToOpenGLBaseType( element.type )
											   , element.normalised ? GL_TRUE : GL_FALSE
											   , static_cast<GLsizei>( layout.GetStride() )
											   , (const void*)( element.offset + sizeof( float ) * count * i )
						);
						glVertexAttribDivisor( opengl_vertexbuffer_index, 1 );
						++opengl_vertexbuffer_index;
					}
					break;
				}

				default:
					ASSERT( false, "Unrecognised ShaderDataType!" );
					break;
			}
		}

		vertex_buffers.push_back( vertex_buffer );
		glBindVertexArray( 0 );
		vertex_buffer->Unbind();
	}

	void VertexArrayOpenGL::SetIndexBuffer( const std::shared_ptr<IndexBuffer>& new_index_buffer )
	{
		ASSERT( !this->index_buffer );

		glBindVertexArray( opengl_vertexarray_id );
		new_index_buffer->Bind();
		glBindVertexArray( 0 );
		new_index_buffer->Unbind();

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
