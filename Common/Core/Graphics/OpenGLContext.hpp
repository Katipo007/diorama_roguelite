#pragma once

namespace Graphics
{
	class OpenGLContext
	{
	public:
		virtual ~OpenGLContext() {}

		virtual void Bind() = 0;
		virtual std::unique_ptr<OpenGLContext> CreateSharedContext() = 0;
	};
}
