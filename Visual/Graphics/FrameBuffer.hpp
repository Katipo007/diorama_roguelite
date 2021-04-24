#pragma once

#include <cinttypes>
#include "Common/Geometry/Size.hpp"

namespace Graphics
{
	struct FrameBufferSpecification
	{
		Size<uint32_t> size = { 0, 0 };

		uint32_t samples = 1;

		bool swap_chain_target = false;
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize( uint32_t width, uint32_t height ) = 0;

		virtual const FrameBufferSpecification& GetSpecification() const = 0;

		virtual uint32_t GetNativeColourAttachment() const = 0;
	};
}
