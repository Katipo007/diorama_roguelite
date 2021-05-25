#pragma once

#include "Common/Core/API/SystemAPI.hpp"

namespace Plugins
{
	class SystemCLI final
		: public ::API::SystemAPI
	{
	public:
		explicit SystemCLI();
		~SystemCLI();

		const Filepath& GetAssetsFilepath() const override;

		std::unique_ptr<Graphics::OpenGLContext> CreateOpenGLContext() override;
		std::shared_ptr<Graphics::Window> CreateWindow( const Graphics::WindowDefinition& definition ) override;
		void DestroyWindow( std::shared_ptr<Graphics::Window> window ) override;

		Size<uint32_t> GetScreenSize() const override;

		std::thread CreateThread( std::string_view name, std::function<void()> runnable ) override;
		void Sleep( unsigned long milliseconds ) override;

		bool GenerateEvents( ::API::VideoAPI* video, ::API::InputAPI* input, ::API::DearImGuiAPI* dearimgui ) override;

		std::string_view GetName() const noexcept override;

	protected:
		void Init() override;
		void Shutdown() override;
		void OnVariableUpdate( const PreciseTimestep&, const StepType ) override;
	};
}
