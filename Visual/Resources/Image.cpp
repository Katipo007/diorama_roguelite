#include "Image.hpp"

namespace Resources
{
	ImageDefinition::ImageDefinition( TexturePtr texture, Rect<float> uv )
		: texture_ptr( texture )
		, uv_rect( uv )
		, cached_size_f( uv.GetWidth(), uv.GetHeight() )
		, cached_size_uint( (unsigned)uv.GetWidth(), (unsigned)uv.GetHeight() )
	{
		ASSERT( texture_ptr != nullptr );
		ASSERT( uv_rect.IsValid() );
	}

	ImageDefinition::~ImageDefinition()
	{
	}


	Image::Image()
	{
	}

	Image::Image( std::shared_ptr<const ImageDefinition> image_def )
		: definition( image_def )
	{
	}

	Image::~Image()
	{
	}

	Image::Image( const Image& to_copy )
		: definition( to_copy.definition )
	{
	}

	Image Image::operator=( const Image& to_copy )
	{
		return Image( to_copy.definition );
	}

	Image::Image( Image&& to_move )
	{
		std::swap( definition, to_move.definition );
	}

	Image& Image::operator=( Image&& to_move )
	{
		if (&to_move != this)
		{
			std::swap( definition, to_move.definition );
		}

		return *this;
	}
}
