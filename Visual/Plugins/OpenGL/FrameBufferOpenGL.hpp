#pragma once

#include "Visual/Graphics/FrameBuffer.hpp"

namespace Graphics::API
{
	class FrameBufferOpenGL
		: public ::Graphics::FrameBuffer
	{
	public:
		FrameBufferOpenGL( const FrameBufferSpecification& spec );
		virtual ~FrameBufferOpenGL() override;

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize( uint32_t width, uint32_t height ) override;

		virtual uint32_t GetNativeColourAttachment() const override { return static_cast<uint32_t>( opengl_colour_attachment ); }
		virtual const FrameBufferSpecification& GetSpecification() const override { return specification; }

	private:
		FrameBufferSpecification specification;
		unsigned int opengl_framebuffer_id;
		unsigned int opengl_colour_attachment;
		unsigned int opengl_depth_attachment;
	};
}
