#pragma once

namespace Visual::Device
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void FrameBegin() = 0;
		virtual void FrameEnd() = 0;
		virtual void SwapBuffers() = 0;
	};
}