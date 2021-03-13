#pragma once

#include "Visual/Device/Texture.hpp"

#ifdef RENDERER_IMPLEMENTATION_OPENGL
#include "OpenGLHeader.hpp"

namespace Visual::Device::OpenGL
{
	class Texture2DOpenGL
		: public ::Visual::Device::Texture2D
	{
	public:
		Texture2DOpenGL( uint32_t width, uint32_t height, const Texture::CreationProperties& props );
		Texture2DOpenGL( std::string_view path, const Texture::LoadProperties& props );
		virtual ~Texture2DOpenGL() override;

		virtual uint32_t GetWidth() const override { return width; }
		virtual uint32_t GetHeight() const override { return height; }
		virtual uint32_t GetNativeId() const override { return opengl_texture_id; }

		virtual void SetData( void* data, uint32_t size ) override;

		virtual void Bind( uint32_t slot ) const override;

		virtual bool operator==( const Texture& other ) const override
		{
			const auto& opengl_other = dynamic_cast<const Texture2DOpenGL&>( other );
			return opengl_texture_id == opengl_other.opengl_texture_id;
		}

	private:
		std::string filepath;
		uint32_t width, height;

		GLenum opengl_internal_format;
		GLenum opengl_data_format;
		GLuint opengl_texture_id;
	};
}

#endif
