#pragma once

#include <cinttypes>
#include <string>

namespace Visual::Device
{
	class Texture
	{
	public:
		enum class TextureWrapSetting
		{
			ClampToEdge,
			Repeat,
			MirroredRepeat,
		};

		struct CreationProperties
		{
			TextureWrapSetting wrap_s = TextureWrapSetting::Repeat;
			TextureWrapSetting wrap_t = TextureWrapSetting::Repeat;

			CreationProperties() {}
		};

		struct LoadProperties
		{
			bool y_flip = true;
			TextureWrapSetting wrap_s = TextureWrapSetting::Repeat;
			TextureWrapSetting wrap_t = TextureWrapSetting::Repeat;

			LoadProperties() {}
		};

	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetNativeId() const = 0;

		virtual void SetData( void* data, uint32_t size ) = 0;

		virtual void Bind( uint32_t slot ) const = 0;

		virtual bool operator==( const Texture& other ) const = 0;
	};

	class Texture2D
		: public Texture
	{
	public:

	};
}
