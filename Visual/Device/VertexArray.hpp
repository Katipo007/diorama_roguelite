#pragma once

#include <memory>
#include <vector>
#include "GraphicsBuffer.hpp"

namespace Visual::Device
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer( const std::shared_ptr<VertexBuffer>& vertex_buffer ) = 0;
		virtual void SetIndexBuffer( const std::shared_ptr<IndexBuffer>& index_buffer ) = 0;

		virtual const std::vector< std::shared_ptr<VertexBuffer> >& GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<VertexBuffer>& GetVertexBuffer( size_t i ) const = 0;
		virtual size_t GetVertexBufferCount() const = 0;

		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;
	};
}
