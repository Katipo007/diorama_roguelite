#pragma once

#include "Common/Core/API/SystemAPI.hpp"

namespace Plugins
{
	class SystemCLI final
		: public ::API::SystemAPI
	{
	public:
		explicit SystemCLI();
		virtual ~SystemCLI();

		virtual const Filepath& GetAssetsFilepath() const override;

		virtual std::unique_ptr<Graphics::OpenGLContext> CreateOpenGLContext() override;
		virtual std::shared_ptr<Graphics::Window> CreateWindow( const Graphics::WindowDefinition& definition ) override;
		virtual void DestroyWindow( std::shared_ptr<Graphics::Window> window ) override;

		virtual Size<uint32_t> GetScreenSize() const override;

		virtual std::thread CreateThread( std::string_view name, std::function<void()> runnable ) override;

		virtual bool GenerateEvents( ::API::VideoAPI* video, ::API::InputAPI* input, ::API::DearImGuiAPI* dearimgui ) override;
		virtual void Update( const PreciseTimestep& ts ) override;

		virtual std::string_view GetName() const noexcept override;

	protected:
		virtual void Init() override;
		virtual void Shutdown() override;
	};
}
