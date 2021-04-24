#pragma once

#include "Common/Core/API/InternalAPI.hpp"

namespace Graphics::API
{
	class WindowSDL2;

	class SystemSDL2 final
		: public ::API::InternalSystemAPI
	{
	public:
		explicit SystemSDL2();
		virtual ~SystemSDL2();

		virtual const Filepath& GetAssetsFilepath() const override;

		virtual std::unique_ptr<Graphics::OpenGLContext> CreateOpenGLContext() override;
		virtual std::shared_ptr<Graphics::Window> CreateWindow( const Graphics::WindowDefinition& definition ) override;
		virtual void DestroyWindow( std::shared_ptr<Graphics::Window> window ) override;

		virtual Size<uint32_t> GetScreenSize() const override;

		virtual std::thread CreateThread( std::string_view name, std::function<void()> runnable ) override;

		virtual bool GenerateEvents( ::API::VideoAPI* video, ::API::InputAPI* input ) override;
		virtual void Update( const PreciseTimestep& ts ) override;

		virtual std::string_view GetName() const override;

	protected:
		virtual void Init() override;
		virtual void Shutdown() override;

		void InitVideo() const;
		void ShutdownVideo();

	protected:
		std::vector<std::shared_ptr<WindowSDL2>> windows;
		mutable bool video_init = false;
	};
}
