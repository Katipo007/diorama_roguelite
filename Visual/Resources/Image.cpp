#include "Image.hpp"

#include "ImageResourceManager.hpp" // where ImageDefinition is defined

namespace
{
	static const std::shared_ptr<const Visual::Device::Texture2D> EmptyTextureHandle = nullptr;
}

namespace Resources
{
	const Image Image::empty = Image();

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

	Image::Image( Image&& to_move ) noexcept
	{
		std::swap( definition, to_move.definition );
	}

	Image& Image::operator=( Image&& to_move ) noexcept
	{
		if (&to_move != this)
		{
			std::swap( definition, to_move.definition );
		}

		return *this;
	}

	const Size<unsigned>& Image::GetSize() const
	{
		return definition ? definition->GetSize() : Size<unsigned>::empty;
	}

	const Size<float>& Image::GetSizeF() const
	{
		return definition ? definition->GetSizeF() : Size<float>::empty;
	}

	const std::shared_ptr<const Visual::Device::Texture2D>& Image::GetSharedTexture() const
	{
		return definition ? definition->GetSharedTexture() : EmptyTextureHandle;
	}

	const Rect<float>& Image::GetUVs() const
	{
		return definition ? definition->GetUVs() : Rect<float>::empty;
	}
}
