#pragma once

#include <cinttypes>
#include <memory>

#include "Visual/Graphics/Colour.hpp"
#include "RendererAPIs.hpp"
#include "RendererCapabilities.hpp"
#include "FrameBuffer.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "GraphicsBuffer.hpp"

namespace Visual::Device
{
	class Shader;

	class RendererAPI
	{
	public:
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void SetViewport( uint32_t x, uint32_t y, uint32_t width, uint32_t height ) = 0;
		virtual uint32_t GetViewportX() = 0;
		virtual uint32_t GetViewportY() = 0;
		virtual uint32_t GetViewportWidth() = 0;
		virtual uint32_t GetViewportHeight() = 0;

		virtual void SetClearColour( ColourRGBA colour ) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed( const std::shared_ptr<VertexArray>& vertex_array, uint32_t index_count = 0 ) = 0;
		virtual const RendererCapabilities& GetCapabilities() const = 0;

		virtual RendererAPIs::API GetAPIType() const = 0;

		_NODISCARD virtual std::shared_ptr<VertexBuffer> CreateVertexBuffer( const VertexBuffer::CreationProperties& props ) const = 0;
		_NODISCARD virtual std::shared_ptr<IndexBuffer> CreateIndexBuffer( const IndexBuffer::CreationProperties& props ) const = 0;
		_NODISCARD virtual std::shared_ptr<FrameBuffer> CreateFrameBuffer( const FrameBuffer::Specification& spec ) const = 0;
		_NODISCARD virtual std::shared_ptr<Shader> CreateShader( const std::filesystem::path& filepath ) const = 0;
		_NODISCARD inline virtual std::shared_ptr<Shader> CreateShader( std::string_view filepath ) const { return CreateShader( std::filesystem::path( filepath ) ); }
		_NODISCARD virtual std::shared_ptr<Shader> CreateShader( std::string_view name, std::string_view vertex_src, std::string_view fragment_src ) const = 0;
		_NODISCARD virtual std::shared_ptr<Texture2D> CreateTexture2D( uint32_t width, uint32_t height, const Texture::CreationProperties& props = Texture::CreationProperties() ) const = 0;
		_NODISCARD virtual std::shared_ptr<Texture2D> CreateTexture2D( const std::filesystem::path& filepath, const Texture::LoadProperties& props = Texture::LoadProperties() ) const = 0;
		_NODISCARD inline virtual std::shared_ptr<Texture2D> CreateTexture2D( std::string_view filepath, const Texture::LoadProperties& props = Texture::LoadProperties() ) const { return CreateTexture2D( std::filesystem::path( filepath ), props ); }
		_NODISCARD virtual std::shared_ptr<VertexArray> CreateVertexArray( const VertexArray::CreationProperties& props ) const = 0;
	};
}