#include "TextureOpenGL.hpp"
#include "OpenGLHeader.hpp"

#include "Common/Utility/Unreachable.hpp"

#include "Visual/Utility/StbImage.hpp"

namespace Graphics::API
{
	namespace
	{
		GLuint ConvertTextureWrapSetting( TextureWrapSetting value )
		{
			switch( value )
			{
			case TextureWrapSetting::ClampToEdge:
				return GL_CLAMP_TO_EDGE;
			case TextureWrapSetting::Repeat:
				return  GL_REPEAT;
			case TextureWrapSetting::MirroredRepeat:
				return GL_MIRRORED_REPEAT;
			}

			unreachable();
		}
	}

	TextureOpenGL::TextureOpenGL( const TextureDefinition& definition )
		: size( definition.size )
	{
		opengl_internal_format = GL_RGBA8;
		opengl_data_format = GL_RGBA;

		glCreateTextures( GL_TEXTURE_2D, 1, &opengl_texture_id );
		glTextureStorage2D( opengl_texture_id, 1, opengl_internal_format, static_cast<GLsizei>( size.width ), static_cast<GLsizei>( size.height ) );

		glTextureParameteri( opengl_texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTextureParameteri( opengl_texture_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		glTextureParameteri( opengl_texture_id, GL_TEXTURE_WRAP_S, ConvertTextureWrapSetting( definition.wrap_s ) );
		glTextureParameteri( opengl_texture_id, GL_TEXTURE_WRAP_T, ConvertTextureWrapSetting( definition.wrap_t ) );
	}

	TextureOpenGL::TextureOpenGL( const Filepath& filepath, const TextureLoadProperties& props )
	{
		ASSERT( std::filesystem::is_regular_file( filepath ) );
		
		const auto _filepath = filepath.string();
		int _out_w, _out_h, _out_channels;
		stbi_set_flip_vertically_on_load( props.y_flip ? 1 : 0 );
		stbi_uc* _data = nullptr;
		_data = stbi_load( _filepath.c_str(), &_out_w, &_out_h, &_out_channels, 0 );

		ASSERT( _data, "Failed to load image" );

		size = Size( (uint32_t)_out_w, (uint32_t)_out_h );

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
		glTextureStorage2D( opengl_texture_id, 1, opengl_internal_format, static_cast<GLsizei>( size.width ), static_cast<GLsizei>( size.height ) );

		glTextureParameteri( opengl_texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTextureParameteri( opengl_texture_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		glTextureParameteri( opengl_texture_id, GL_TEXTURE_WRAP_S, ConvertTextureWrapSetting( props.wrap_s ) );
		glTextureParameteri( opengl_texture_id, GL_TEXTURE_WRAP_T, ConvertTextureWrapSetting( props.wrap_t ) );

		glTextureSubImage2D( opengl_texture_id, 0, 0, 0, static_cast<GLsizei>( size.width ), static_cast<GLsizei>( size.height ), opengl_data_format, GL_UNSIGNED_BYTE, _data );

		stbi_image_free( _data );
		this->filepath = filepath.string();
	}

	TextureOpenGL::~TextureOpenGL()
	{
		glDeleteTextures( 1, &opengl_texture_id );
	}

	void TextureOpenGL::SetData( void * data, uint32_t data_size )
	{
		uint32_t _bpp = ( opengl_data_format == GL_RGBA ) ? 4 : 3;
		ASSERT( data_size == size.width * size.height * _bpp, "Data size must exactly match texture!" );
		glTextureSubImage2D( opengl_texture_id, 0, 0, 0, static_cast<GLsizei>( size.width ), static_cast<GLsizei>( size.height ), opengl_data_format, GL_UNSIGNED_BYTE, data );
	}

	void TextureOpenGL::Bind( uint32_t slot ) const
	{
		glBindTextureUnit( static_cast<GLenum>( slot ), opengl_texture_id );
	}

	bool TextureOpenGL::operator==( const Texture& other ) const
	{
		const auto& opengl_other = dynamic_cast<const TextureOpenGL&>(other);
		return opengl_texture_id == opengl_other.opengl_texture_id;
	}
}
