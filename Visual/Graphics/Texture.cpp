#include "Texture.hpp"

#include "Common/Core/Core.hpp"
#include "Common/Core/ResourceManager.hpp"
#include "Common/Core/API/VideoAPI.hpp"
#include "Common/Core/Resources/ResourceLoader.hpp"

namespace Graphics
{
	std::shared_ptr<Texture> Texture::LoadResource( ::Resources::ResourceLoader& loader )
	{
		std::shared_ptr<Texture> texture;

		auto& core = loader.GetManager().GetCore();
		auto video = core.GetAPI<::API::VideoAPI>();
		ASSERT( video );

		Filepath filepath = loader.GetAssetId();

		// TODO: some way of storing/fetching metadata

		TextureLoadProperties props;
		props.y_flip = false;
		texture = video->CreateTexture( filepath, props );

		return texture;
	}
}
