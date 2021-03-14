#pragma once

#include "Visual/Device/GraphicsBuffer.hpp"

#ifdef RENDERER_IMPLEMENTATION_OPENGL

namespace Visual::Device::OpenGL
{
	class VertexBufferOpenGL
		: public ::Visual::Device::VertexBuffer
	{
	public:
		VertexBufferOpenGL( const CreationProperties& props );
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
		: public ::Visual::Device::IndexBuffer
	{
	public:
		IndexBufferOpenGL( const CreationProperties& props );
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

#endif
