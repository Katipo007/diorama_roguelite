#include "Texture.hpp"

#include "Common/Core/Resources/IResourceLoader.hpp"

#include "Visual/Device/Texture.hpp"
#include "Visual/Device/RendererAPI.hpp"
#include "Visual/Device/RendererCommand.hpp"

namespace Resources
{
	Texture::Texture( Size<uint16_t> size )
		: size( size )
	{
	}

	std::shared_ptr<Texture> Texture::LoadResource( IResourceLoader& loader )
	{
		auto& renderer_api = Visual::Device::RendererCommand::GetRendererAPI();

		Visual::Device::Texture2D::LoadProperties texture_props;
		
		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		texture->device_texture = renderer_api.CreateTexture2D( loader.GetAssetId(), texture_props );
		
		return texture;
	}
}
