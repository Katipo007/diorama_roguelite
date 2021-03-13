#pragma once

#include "Visual/Device/RendererAPI.hpp"

#ifdef RENDERER_IMPLEMENTATION_OPENGL

namespace Visual::Device::OpenGL
{
	class RendererAPIOpenGL
		: public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void SetViewport( uint32_t x, uint32_t y, uint32_t w, uint32_t h ) override;
		virtual uint32_t GetViewportX() override;
		virtual uint32_t GetViewportY() override;
		virtual uint32_t GetViewportWidth() override;
		virtual uint32_t GetViewportHeight() override;

		virtual void SetClearColour( ColourRGBA colour ) override;
		virtual void Clear() override;

		virtual void DrawIndexed( const std::shared_ptr<VertexArray>& vertex_array, uint32_t index_count = 0 ) override;

		virtual const RendererCapabilities& GetCapabilities() const override;

		virtual RendererAPIs::API GetAPIType() const override { return RendererAPIs::OpenGL; }

		// Graphics object factories
		virtual std::shared_ptr<VertexBuffer> CreateVertexBuffer( uint32_t size ) const override;
		virtual std::shared_ptr<VertexBuffer> CreateVertexBuffer( float * vertices, uint32_t size ) const override;
		virtual std::shared_ptr<IndexBuffer> CreateIndexBuffer( uint32_t * indices, uint32_t size ) const override;
		virtual std::shared_ptr<FrameBuffer> CreateFrameBuffer( const FrameBuffer::Specification & spec ) const override;
		virtual std::shared_ptr<Shader> CreateShader( std::string_view filepath ) const override;
		virtual std::shared_ptr<Shader> CreateShader( std::string_view name, std::string_view vertex_src, std::string_view fragment_src ) const override;
		virtual std::shared_ptr<Texture2D> CreateTexture2D( uint32_t width, uint32_t height, const Texture::CreationProperties & props = Texture::CreationProperties() ) const override;
		virtual std::shared_ptr<Texture2D> CreateTexture2D( std::string_view filepath, const Texture::LoadProperties & props = Texture::LoadProperties() ) const override;
		virtual std::shared_ptr<VertexArray> CreateVertexArray() const override;

	private:
		RendererCapabilities cached_capabilities;
	};
}

#endif
