#pragma once

#include <cinttypes>
#include <string>

#include "Common/Geometry/Size.hpp"
#include "Common/Core/Resources/Resource.hpp"
#include "Common/Core/Resources/ResourceTypes.hpp"

namespace Resources { class ResourceLoader; }

namespace Graphics
{
	enum class TextureWrapSetting
	{
		ClampToEdge,
		Repeat,
		MirroredRepeat,
	};

	struct TextureDefinition
	{
		Size<uint32_t> size;
		TextureWrapSetting wrap_s = TextureWrapSetting::Repeat;
		TextureWrapSetting wrap_t = TextureWrapSetting::Repeat;
	};

	struct TextureLoadProperties
	{
		bool y_flip = true;
		TextureWrapSetting wrap_s = TextureWrapSetting::Repeat;
		TextureWrapSetting wrap_t = TextureWrapSetting::Repeat;
	};

	class Texture
		: public ::Resources::Resource
	{
	public:
		virtual ~Texture() = default;

		static constexpr ::Resources::AssetType GetResourceType() noexcept { return ::Resources::AssetType::Texture; }
		static std::shared_ptr<Texture> LoadResource( ::Resources::ResourceLoader& loader );

		virtual const Size<uint32_t>& GetSize() const noexcept = 0;

		virtual void SetData( void* data, uint32_t size ) = 0;

		virtual void Bind( uint32_t slot ) const = 0;

		virtual bool operator==( const Texture& other ) const = 0;

		virtual uint32_t GetNativeId() const noexcept = 0;
	};
}
