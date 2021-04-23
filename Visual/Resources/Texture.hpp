#pragma once

#include "Common/Geometry/Size.hpp"
#include "Common/Core/Resources/Resource.hpp"
#include "Common/Core/Resources/ResourceTypes.hpp"

namespace Visual::Device
{
	class Texture2D;
}

namespace Resources
{
	class Texture
		: public Resource
	{
	public:
		Texture( Size<uint16_t> size );
		virtual ~Texture();

		constexpr static AssetType GetResourceType() { return AssetType::Texture; }

		static std::shared_ptr<Texture> LoadResource( IResourceLoader& loader );

		std::shared_ptr<const Visual::Device::Texture2D> GetDeviceTexture() const { return device_texture; }
		const Size<uint16_t>& GetSize() const { return size; }

	protected:
		std::shared_ptr<const Visual::Device::Texture2D> device_texture;
		const Size<uint16_t> size;
	};
}
