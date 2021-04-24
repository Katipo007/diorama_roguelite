#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Common/File/Filepath.hpp"
#include "Common/Geometry/Rect.hpp"
#include "Common/Geometry/Size.hpp"
#include "Common/Utility/json.hpp"

#include "Image.hpp"

namespace API { class VideoAPI; }

namespace Graphics { class Texture; }

namespace Resources
{
	class ImageDefinition
	{
	protected:
		using ConstTexturePtr = std::shared_ptr<const Graphics::Texture>;
	public:
		ImageDefinition( ConstTexturePtr texture, Rect<float> uv );
		virtual ~ImageDefinition();

		const Rect<float>& GetUVs() const { return uv_rect; }
		const ConstTexturePtr& GetSharedTexture() const { return texture_ptr; }

		const Size<unsigned>& GetSize() const { return cached_size_uint; }
		const Size<float>& GetSizeF() const { return cached_size_f; }

	protected:
		Rect<float> uv_rect = Rect<float>::empty;
		Size<float> cached_size_f;
		Size<unsigned> cached_size_uint;

		ConstTexturePtr texture_ptr;
	};

	/// <summary>
	/// Stores
	/// </summary>
	class ImageResourceManager final
	{
	public:
		explicit ImageResourceManager( ::API::VideoAPI& video );
		~ImageResourceManager();

#pragma region Retrieving images
		Image GetImageById( std::string_view image_id ) const;
#pragma endregion


#pragma region Adding images to manager
		bool AddImagesFromJson( std::string_view json_string, const Filepath& filepath_prefix = std::filesystem::current_path() );
		bool AddImagesFromFile( const Filepath& filepath );
#pragma endregion

	protected:
		bool ParseJsonFreeTexPacker( const nlohmann::json& json, const Filepath& filepath_prefix = std::filesystem::current_path() );
		bool AddImageDefinition( std::string_view image_id, std::shared_ptr<Graphics::Texture> texture, const Rect<float>& uvs );

	protected:
		::API::VideoAPI& video;
		std::unordered_map<std::string, const std::shared_ptr<const ImageDefinition>> id_to_image_def;
	};
}
