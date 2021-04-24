#pragma once

#include "Common/Core/API/VideoAPI.hpp"
#include "Visual/Graphics/DeviceCapabilities.hpp"

namespace API { class SystemAPI; }
namespace Graphics { class OpenGLContext; }

namespace Graphics::API
{
	class VideoOpenGL final
		: public ::API::VideoAPI
	{
	public:
		VideoOpenGL( ::API::SystemAPI& system );
		~VideoOpenGL();

		virtual void BeginRender() override;
		virtual void EndRender() override;

		virtual void SetWindow( Graphics::WindowDefinition&& definition ) override;
		virtual const Graphics::Window& GetWindow() const override;
		virtual bool HasWindow() const override;
		virtual void SetVSync( const bool ) override;

		virtual void SetViewport( Rect<uint32_t> ) override;
		virtual Rect<uint32_t> GetViewport() const override;

		virtual const Graphics::DeviceCapabilities& GetDeviceCapabilities() const override { return capabilities; }

		virtual void DrawIndexed( const std::shared_ptr<Graphics::VertexArray>& vertex_array, uint32_t index_count = 0 ) override;

		virtual std::shared_ptr<Graphics::VertexBuffer> CreateVertexBuffer( const Graphics::VertexBufferDefinition& definition ) const override;
		virtual std::shared_ptr<Graphics::IndexBuffer> CreateIndexBuffer( const Graphics::IndexBufferDefinition& definition ) const override;
		virtual std::shared_ptr<Graphics::FrameBuffer> CreateFrameBuffer( const Graphics::FrameBufferSpecification& definition ) const override;
		virtual std::shared_ptr<Graphics::Shader> CreateShader( const Filepath& filepath ) const override;
		virtual std::shared_ptr<Graphics::Shader> CreateShader( std::string_view name, std::string_view vertex_src, std::string_view fragment_src ) const override;
		virtual std::shared_ptr<Graphics::Texture> CreateTexture( const Graphics::TextureDefinition& props ) const override;
		virtual std::shared_ptr<Graphics::Texture> CreateTexture( const Filepath& filepath, const Graphics::TextureLoadProperties& props ) const override;
		virtual std::shared_ptr<Graphics::VertexArray> CreateVertexArray( const Graphics::VertexArrayDefinition& definition ) const override;

		virtual std::string_view GetName() const noexcept override;
		virtual std::string_view GetShaderLanguage() const override;

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

		DeviceCapabilities capabilities;

		std::shared_ptr<::Graphics::Window> window;
		bool vsync_enabled = false;
	};
}
