#pragma once

#include "Visual/Graphics/GraphicsBuffer.hpp"

namespace Graphics::API
{
	class VertexBufferOpenGL
		: public ::Graphics::VertexBuffer
	{
	public:
		VertexBufferOpenGL( const VertexBufferDefinition& props );
		virtual ~VertexBufferOpenGL();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData( const void* data, uint32_t size ) override;

		virtual const BufferLayout& GetLayout() const override { return layout; }
		virtual void SetLayout( const BufferLayout& layout ) override;

	private:
		std::string name;
		uint32_t vbo;
		BufferLayout layout;
	};

	class IndexBufferOpenGL
		: public ::Graphics::IndexBuffer
	{
	public:
		IndexBufferOpenGL( const IndexBufferDefinition& props );
		virtual ~IndexBufferOpenGL();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const { return count; }
	private:
		std::string name;
		uint32_t ibo;
		uint32_t count;
	};
}
