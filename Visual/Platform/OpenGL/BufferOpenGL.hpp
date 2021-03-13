#pragma once

#include "Visual/Device/GraphicsBuffer.hpp"

#ifdef RENDERER_IMPLEMENTATION_OPENGL

namespace Visual::Device::OpenGL
{
	class VertexBufferOpenGL
		: public ::Visual::Device::VertexBuffer
	{
	public:
		VertexBufferOpenGL( uint32_t size );
		VertexBufferOpenGL( float* vertices, uint32_t size );
		virtual ~VertexBufferOpenGL();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData( const void* data, uint32_t size ) override;

		virtual const BufferLayout& GetLayout() const override { return layout; }
		virtual void SetLayout( const BufferLayout& layout ) override;

	private:
		uint32_t opengl_vb_id;
		BufferLayout layout;
	};

	class IndexBufferOpenGL
		: public ::Visual::Device::IndexBuffer
	{
	public:
		IndexBufferOpenGL( uint32_t* indices, uint32_t count );
		virtual ~IndexBufferOpenGL();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const { return count; }
	private:
		uint32_t opengl_ib_id;
		uint32_t count;
	};
}

#endif
