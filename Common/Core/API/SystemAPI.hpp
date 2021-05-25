#pragma once

#include "CoreAPIsEnum.hpp"
#include "BaseAPI.hpp"
#include "Common/Geometry/Size.hpp"
#include "Common/File/Filepath.hpp"

namespace Graphics
{
	class Window;
	struct WindowDefinition;
	class OpenGLContext;
}

namespace API
{
	class DearImGuiAPI;
	class InputAPI;
	class VideoAPI;

	class SystemAPI
		: public BaseAPI
	{
		friend class ::Core;

	public:
		virtual ~SystemAPI() {}

		static constexpr APIType GetType() noexcept { return CoreAPIs::System; }

		virtual const Filepath& GetAssetsFilepath() const = 0;

		virtual std::unique_ptr<Graphics::OpenGLContext> CreateOpenGLContext() = 0;
		virtual std::shared_ptr<Graphics::Window> CreateWindow( const Graphics::WindowDefinition& definition ) = 0;
		virtual void DestroyWindow( std::shared_ptr<Graphics::Window> window ) = 0;

		virtual Size<uint32_t> GetScreenSize() const = 0;

		virtual std::thread CreateThread( std::string_view name, std::function<void()> runnable ) = 0;
		virtual void SetThreadName( std::string name ) {}
		virtual void Sleep( unsigned long milliseconds ) = 0;

	private:
		virtual bool GenerateEvents( VideoAPI* video, InputAPI* input, DearImGuiAPI* dearimgui ) = 0;
	};
}
