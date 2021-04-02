#pragma once

#include <string>
#include <optional>

#include "Common/Geometry/Rect.hpp"
#include "Common/Geometry/Size.hpp"
#include "Visual/Device/Texture.hpp"

namespace Visual::Device
{
	class Texture2D;
}

namespace Resources
{
	class ImageDefinition;

	class Image
	{
	public:
		static const Image empty;

	public:
		Image();
		Image( std::shared_ptr<const ImageDefinition> image_def );
		virtual ~Image();

		Image( const Image& to_copy );
		Image operator=( const Image& to_copy );
		Image( Image&& to_move ) noexcept;
		Image& operator=( Image&& to_move ) noexcept;

		bool IsNull() const { return definition != nullptr; }
		operator bool() const { return !IsNull(); }

		const Size<unsigned>& GetSize() const;
		const Size<float>& GetSizeF() const;

		const std::shared_ptr<const Visual::Device::Texture2D>& GetSharedTexture() const;
		const Rect<float>& GetUVs() const;

	protected:
		std::shared_ptr<const ImageDefinition> definition;
	};
}
