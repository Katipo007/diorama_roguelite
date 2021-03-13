#include "Precompiled/pch.hpp"
#include "TextureOpenGL.hpp"


#ifdef RENDERER_IMPLEMENTATION_OPENGL

#include "Visual/Utility/StbImage.hpp"

namespace Visual::Device::OpenGL
{
	namespace
	{
		GLuint ConvertTextureWrapSetting( Texture::TextureWrapSetting value )
		{
			switch( value )
			{
			case Texture::TextureWrapSetting::ClampToEdge:
				return GL_CLAMP_TO_EDGE;
			case Texture::TextureWrapSetting::Repeat:
				return  GL_REPEAT;
				break;
			case Texture::TextureWrapSetting::MirroredRepeat:
				return GL_MIRRORED_REPEAT;
				break;
			}

			UNREACHABLE;
			return GL_REPEAT;
		}
	}

	Texture2DOpenGL::Texture2DOpenGL( uint32_t width_, uint32_t height_, const Texture::CreationProperties& props )
		: width( width_ )
		, height( height_ )
	{
		opengl_internal_format = GL_RGBA8;
		opengl_data_format = GL_RGBA;

		glCreateTextures( GL_TEXTURE_2D, 1, &opengl_texture_id );
		glTextureStorage2D( opengl_texture_id, 1, opengl_internal_format, static_cast<GLsizei>( width ), static_cast<GLsizei>( height ) );

		glTextureParameteri( opengl_texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTextureParameteri( opengl_texture_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		glTextureParameteri( opengl_texture_id, GL_TEXTURE_WRAP_S, ConvertTextureWrapSetting( props.wrap_s ) );
		glTextureParameteri( opengl_texture_id, GL_TEXTURE_WRAP_T, ConvertTextureWrapSetting( props.wrap_t ) );
	}

	Texture2DOpenGL::Texture2DOpenGL( std::string_view path, const Texture::LoadProperties& props )
	{
		const auto _filepath = ( std::string )path;
		int _out_w, _out_h, _out_channels;
		stbi_set_flip_vertically_on_load( props.y_flip ? 1 : 0 );
		stbi_uc* _data = nullptr;
		_data = stbi_load( _filepath.c_str(), &_out_w, &_out_h, &_out_channels, 0 );

		ASSERT( _data, "Failed to load image" );

		width = static_cast<uint32_t>( _out_w );
		height = static_cast<uint32_t>( _out_h );

		GLenum _internal_format = 0;
		GLenum _data_format = 0;
		if( _out_channels == 4 )
		{
			_internal_format = GL_RGBA8;
			_data_format = GL_RGBA;
		}
		else if( _out_channels == 3 )
		{
			_internal_format = GL_RGB8;
			_data_format = GL_RGB;
		}
		else if( _out_channels == 1 )
		{
			_internal_format = GL_R8;
			_data_format = GL_RED;
		}
		else
			ASSERT( false, "Unsupported number of channels in image!" );

		opengl_internal_format = _internal_format;
		opengl_data_format = _data_format;

		ASSERT( opengl_internal_format & opengl_data_format, "Unsupported image format!" );

		glCreateTextures( GL_TEXTURE_2D, 1, &opengl_texture_id );
		glTextureStorage2D( opengl_texture_id, 1, opengl_internal_format, static_cast<GLsizei>( width ), static_cast<GLsizei>( height ) );

		glTextureParameteri( opengl_texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTextureParameteri( opengl_texture_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		glTextureParameteri( opengl_texture_id, GL_TEXTURE_WRAP_S, ConvertTextureWrapSetting( props.wrap_s ) );
		glTextureParameteri( opengl_texture_id, GL_TEXTURE_WRAP_T, ConvertTextureWrapSetting( props.wrap_t ) );

		glTextureSubImage2D( opengl_texture_id, 0, 0, 0, static_cast<GLsizei>( width ), static_cast<GLsizei>( height ), opengl_data_format, GL_UNSIGNED_BYTE, _data );

		stbi_image_free( _data );
	}

	Texture2DOpenGL::~Texture2DOpenGL()
	{
		glDeleteTextures( 1, &opengl_texture_id );
	}

	void Texture2DOpenGL::SetData( void * data, uint32_t size )
	{
		uint32_t _bpp = ( opengl_data_format == GL_RGBA ) ? 4 : 3;
		ASSERT( size == width * height * _bpp, "Data size must exactly match texture!" );
		glTextureSubImage2D( opengl_texture_id, 0, 0, 0, static_cast<GLsizei>( width ), static_cast<GLsizei>( height ), opengl_data_format, GL_UNSIGNED_BYTE, data );
	}

	void Texture2DOpenGL::Bind( uint32_t slot ) const
	{
		glBindTextureUnit( static_cast<GLenum>( slot ), opengl_texture_id );
	}
}

#endif
