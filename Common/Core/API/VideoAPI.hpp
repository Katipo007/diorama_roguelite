#pragma once

#include <string>

#include "CoreAPIsEnum.hpp"
#include "BaseAPI.hpp"
#include "Common/File/Filepath.hpp"
#include "Common/Geometry/Size.hpp"
#include "Common/Geometry/Rect.hpp"

namespace Graphics
{
	class Texture;
	struct TextureDefinition;
	struct TextureLoadProperties;
	class Shader;
	class FrameBuffer;
	struct FrameBufferSpecification;
	class IndexBuffer;
	struct IndexBufferDefinition;
	class VertexArray;
	struct VertexArrayDefinition;
	class VertexBuffer;
	struct VertexBufferDefinition;

	class Window;
	struct WindowDefinition;

	struct DeviceCapabilities;
}

namespace API
{
	class VideoAPI
		: public BaseAPI
	{
	public:
		virtual ~VideoAPI() {}

		static constexpr APIType GetType() noexcept { return CoreAPIs::Video; }

		virtual void BeginRender() = 0;
		virtual void EndRender() = 0;

		virtual void SetWindow( Graphics::WindowDefinition&& definition ) = 0;
		virtual const Graphics::Window& GetWindow() const = 0;
		virtual bool HasWindow() const = 0;
		virtual void SetVSync( const bool ) {}

		virtual void SetViewport( Rect<uint32_t> ) = 0;
		virtual Rect<uint32_t> GetViewport() const = 0;

		virtual const Graphics::DeviceCapabilities& GetDeviceCapabilities() const = 0;

		virtual void DrawIndexed( const std::shared_ptr<Graphics::VertexArray>& vertex_array, uint32_t index_count = 0 ) = 0;

		[[nodiscard]] virtual std::shared_ptr<Graphics::VertexBuffer> CreateVertexBuffer( const Graphics::VertexBufferDefinition& definition ) const = 0;
		[[nodiscard]] virtual std::shared_ptr<Graphics::IndexBuffer> CreateIndexBuffer( const Graphics::IndexBufferDefinition& definition ) const = 0;
		[[nodiscard]] virtual std::shared_ptr<Graphics::FrameBuffer> CreateFrameBuffer( const Graphics::FrameBufferSpecification& definition ) const = 0;
		[[nodiscard]] virtual std::shared_ptr<Graphics::Shader> CreateShader( const Filepath& filepath ) const = 0;
		[[nodiscard]] inline std::shared_ptr<Graphics::Shader> CreateShader( std::string_view filepath ) const { return CreateShader( Filepath( filepath ) ); }
		[[nodiscard]] virtual std::shared_ptr<Graphics::Shader> CreateShader( std::string_view name, std::string_view vertex_src, std::string_view fragment_src ) const = 0;
		[[nodiscard]] virtual std::shared_ptr<Graphics::Texture> CreateTexture( const Graphics::TextureDefinition& props ) const = 0;
		[[nodiscard]] virtual std::shared_ptr<Graphics::Texture> CreateTexture( const Filepath& filepath, const Graphics::TextureLoadProperties& props ) const = 0;
		[[nodiscard]] inline std::shared_ptr<Graphics::Texture> CreateTexture( std::string_view filepath, const Graphics::TextureLoadProperties& props ) const { return CreateTexture( Filepath( filepath ), props ); }
		[[nodiscard]] virtual std::shared_ptr<Graphics::VertexArray> CreateVertexArray( const Graphics::VertexArrayDefinition& definition ) const = 0;

		virtual std::string_view GetShaderLanguage() const = 0;

		virtual void* GetImplementationPointer( std::string_view id ) { (void)id; return nullptr; }
	};
}
