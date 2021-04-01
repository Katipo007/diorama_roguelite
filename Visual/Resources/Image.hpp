#pragma once

#include <string>
#include <optional>

#include "Common/Geometry/Rect.hpp"
#include "Common/Geometry/Size.hpp"
#include "Visual/Device/Texture.hpp"

namespace Resources
{
	class ImageDefinition
	{
	protected:
		using TexturePtr = std::shared_ptr<Visual::Device::Texture2D>;
	public:
		ImageDefinition( TexturePtr texture, Rect<float> uv );
		virtual ~ImageDefinition();

		const Rect<float> GetUVs() const { return uv_rect; }
		TexturePtr GetSharedTexture() const { return texture_ptr; }

		const Size<unsigned>& GetSize() const { return cached_size_uint; }
		const Size<float>& GetSizeF() const { return cached_size_f; }

	protected:
		Rect<float> uv_rect = Rect<float>::empty;
		Size<float> cached_size_f;
		Size<unsigned> cached_size_uint;

		TexturePtr texture_ptr;
	};

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
		Image( Image&& to_move );
		Image& operator=( Image&& to_move );

		bool IsNull() const { return definition != nullptr; }
		operator bool() const { return !IsNull(); }

		const Size<unsigned>& GetSize() const { return definition ? definition->GetSize() : Size<unsigned>::empty; }
		const Size<float>& GetSizeF() const { return definition ? definition->GetSizeF() : Size<float>::empty; }

	protected:
		std::shared_ptr<const ImageDefinition> definition;
	};
}
