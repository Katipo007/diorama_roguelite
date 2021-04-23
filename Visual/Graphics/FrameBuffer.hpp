#pragma once

#include <cinttypes>

namespace Graphics
{
	class FrameBuffer
	{
	public:
		struct Specification
		{
			uint32_t width = 0;
			uint32_t height = 0;

			uint32_t samples = 1;

			bool swap_chain_target = false;
		};

	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize( uint32_t width, uint32_t height ) = 0;

		virtual const Specification& GetSpecification() const = 0;

		virtual uint32_t GetNativeColourAttachment() const = 0;
	};
}
