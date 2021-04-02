#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

#include "Common/Geometry/Rect.hpp"
#include "Common/Geometry/Size.hpp"
#include "Common/Utility/json.hpp"

#include "Image.hpp"

namespace Visual::Device
{
	class Texture2D;
}

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

	/// <summary>
	/// Stores
	/// </summary>
	class ImageResourceManager final
	{
	public:
		explicit ImageResourceManager();
		~ImageResourceManager();

#pragma region Retrieving images
		Image GetImageById( std::string_view image_id ) const;
#pragma endregion


#pragma region Adding images to manager
		bool AddImagesFromJson( std::string_view json_string, const std::filesystem::path& filepath_prefix = std::filesystem::current_path() );
		bool AddImagesFromFile( const std::filesystem::path& filepath );
#pragma endregion

	protected:
		bool ParseJsonFreeTexPacker( const nlohmann::json& json, const std::filesystem::path& filepath_prefix = std::filesystem::current_path() );
		bool AddImageDefinition( std::string_view image_id, std::shared_ptr<Visual::Device::Texture2D> texture, const Rect<float>& uvs );

	protected:
		std::unordered_map<std::string, const std::shared_ptr<const ImageDefinition>> id_to_image_def;
	};
}
