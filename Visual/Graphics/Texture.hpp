#pragma once

#include <cinttypes>
#include <string>

#include "Common/Geometry/Size.hpp"

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
	{
	public:
		virtual ~Texture() = default;

		virtual Size<uint32_t> GetSize() const = 0;

		virtual void SetData( void* data, uint32_t size ) = 0;

		virtual void Bind( uint32_t slot ) const = 0;

		virtual bool operator==( const Texture& other ) const = 0;

		virtual uint32_t GetNativeId() const = 0;
	};
}
