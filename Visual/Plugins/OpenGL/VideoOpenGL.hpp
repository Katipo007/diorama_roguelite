#pragma once

#include "Common/Core/API/InternalAPI.hpp"

namespace Graphics::API
{
	class VideoOpenGL final
		: public ::API::InternalVideoAPI
	{
	public:
		VideoOpenGL( ::API::SystemAPI& system );
		~VideoOpenGL();

		void BeginRender() override;
		void EndRender() override;

		void SetWindow( Graphics::WindowDefinition&& definition ) override;
		const Graphics::Window& GetWindow() const override;
		bool HasWindow() const override;
		void SetVSync( const bool ) override;

		std::unique_ptr<Graphics::Texture> CreateTexture( Size<uint16_t> size ) override;
		std::unique_ptr<Graphics::Shader> CreateShader( const Graphics::ShaderDefinition& definition ) override;

		std::string_view GetShaderLanguage() const override;

	protected:
		void Init() override;
		void Shutdown() override;

	private:
		void InitOpenGL();
		void SetupDebugMessageCallback();

		void ClearScreen();
		void SwapFrameBuffers();

		void OnOpenGLDebugMessage( unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char* message ) const;

	private:
		::API::SystemAPI& system;
		std::unique_ptr<::Graphics::OpenGLContext> context;
		bool initialised = false;

		std::shared_ptr<::Graphics::Window> window;
		bool vsync_enabled = false;
	};
}
