#pragma once

#include "Visual/Device/FrameBuffer.hpp"

#ifdef RENDERER_IMPLEMENTATION_OPENGL
#include "OpenGLHeader.hpp"

namespace Visual::Device::OpenGL
{
	class FrameBufferOpenGL
		: public ::Visual::Device::FrameBuffer
	{
	public:
		FrameBufferOpenGL( const FrameBuffer::Specification& spec );
		virtual ~FrameBufferOpenGL() override;

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize( uint32_t width, uint32_t height ) override;

		virtual uint32_t GetNativeColourAttachment() const override { return static_cast<uint32_t>( opengl_colour_attachment ); }
		virtual const FrameBuffer::Specification& GetSpecification() const override { return specification; }

	private:
		FrameBuffer::Specification specification;
		GLuint opengl_framebuffer_id;
		GLuint opengl_colour_attachment;
		GLuint opengl_depth_attachment;
	};
}

#endif
