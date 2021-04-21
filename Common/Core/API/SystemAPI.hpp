#pragma once

#include "Common/Geometry/Size.hpp"
#include "Common/File/Filepath.hpp"
#include "Common/Utility/Timestep.hpp"

class Core;

namespace Graphics
{
	class Window;
	struct WindowDefinition;
	class OpenGLContext;
}

namespace API
{
	class InputAPI;
	class VideoAPI;

	class SystemAPI
	{
		friend class ::Core;

	public:
		virtual ~SystemAPI() {}

		virtual const Filepath& GetAssetsFilepath() const = 0;

		virtual std::unique_ptr<Graphics::OpenGLContext> CreateOpenGLContext() = 0;
		virtual std::shared_ptr<Graphics::Window> CreateWindow( const Graphics::WindowDefinition& definition ) = 0;
		virtual void DestroyWindow( std::shared_ptr<Graphics::Window> window ) = 0;

		virtual Size<uint32_t> GetScreenSize() const = 0;

		virtual std::thread CreateThread( std::string_view name, std::function<void()> runnable ) = 0;
		virtual void SetThreadName( std::string name ) {}

	private:
		virtual bool GenerateEvents( VideoAPI* video, InputAPI* input ) = 0;
	};
}
