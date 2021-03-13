#pragma once

#include "Visual/Device/VertexArray.hpp"

#ifdef RENDERER_IMPLEMENTATION_OPENGL
#include "OpenGLHeader.hpp"

namespace Visual::Device::OpenGL
{
	class VertexArrayOpenGL
		: public ::Visual::Device::VertexArray
	{
	public:
		VertexArrayOpenGL();
		virtual ~VertexArrayOpenGL() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer( const std::shared_ptr<VertexBuffer>& vertex_buffer ) override;
		virtual void SetIndexBuffer( const std::shared_ptr<IndexBuffer>& index_buffer ) override;

		virtual const std::vector< std::shared_ptr<VertexBuffer> >& GetVertexBuffers() const override { return vertex_buffers; }
		virtual const std::shared_ptr<VertexBuffer>& GetVertexBuffer( size_t i ) const override;
		virtual size_t GetVertexBufferCount() const override;

		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return index_buffer; }

	private:
		GLuint opengl_vertexarray_id;
		GLuint opengl_vertexbuffer_index;

		std::vector< std::shared_ptr< VertexBuffer > > vertex_buffers;
		std::shared_ptr< IndexBuffer > index_buffer;
	};
}

#endif