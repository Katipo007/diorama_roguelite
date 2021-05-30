#include "ImageResourceManager.hpp"

#include "Common/Core/API/VideoAPI.hpp"
#include "Common/File/FileOps.hpp"
#include "Common/Utility/json.hpp"

#include "Visual/Graphics/Texture.hpp"

namespace Resources
{
	ImageDefinition::ImageDefinition( ConstTexturePtr texture, Rect<float> uv )
		: texture_ptr( texture )
		, cached_size_f( uv.GetWidth(), uv.GetHeight() )
		, cached_size_uint( (unsigned)cached_size_f.width, (unsigned)cached_size_f.height )
	{
		ASSERT( texture_ptr != nullptr );
		ASSERT( uv_rect.IsValid() );

		const auto texture_size = texture->GetSize();
		uv_rect = { uv.GetLeft() / texture_size.width, uv.GetTop() / texture_size.height, uv.GetRight() / texture_size.width, uv.GetBottom() / texture_size.height };
	}

	ImageDefinition::~ImageDefinition()
	{
	}


	/// 
	/// ImageResourceManager
	/// 

	ImageResourceManager::ImageResourceManager( ::API::VideoAPI& video_ )
		: video( video_ )
	{
	}

	ImageResourceManager::~ImageResourceManager()
	{
		id_to_image_def.clear();
	}

	Image ImageResourceManager::GetImageById( std::string_view image_id ) const
	{
		// Using this to cut down on std::string allocations per lookup.
		// Once c++20 is stable should replace with the new options they have.
		thread_local static std::string _working_str;
		_working_str.reserve( image_id.size() );
		_working_str.assign( image_id.data(), image_id.size() );

		const auto it = id_to_image_def.find( _working_str );
		if (it != std::end( id_to_image_def ))
			return Image( it->second );

		return Image::empty;
	}

	bool ImageResourceManager::AddImagesFromJson( std::string_view json_string, const std::filesystem::path& filepath_prefix )
	{
		ASSERT( std::filesystem::is_directory( filepath_prefix ) );
		auto j = nlohmann::json::parse( json_string );
		bool handled = false;

		try
		{
			if (j.contains( "frames" ) && j.contains( "meta" ) && j["meta"]["app"].is_string() && j["meta"]["app"].get<std::string>() == "http://free-tex-packer.com")
				handled = ParseJsonFreeTexPacker( j, filepath_prefix );
		}
		catch (std::exception& e)
		{
			LOG_ERROR( LoggingChannels::Application, "Exception while parsing images from json: '{}'", e.what() );
			handled = false;
		}

		if (!handled)
			LOG_ERROR( LoggingChannels::Application, "Failed to add images from json" );
		return handled;
	}

	bool ImageResourceManager::AddImagesFromFile( const std::filesystem::path& filepath )
	{
		const auto file_directory = FileOps::GetFileDirectory( filepath, true );

		std::string file_contents;
		if (FileOps::ReadFile( filepath, file_contents ))
			return AddImagesFromJson( file_contents, file_directory );

		LOG_WARN( LoggingChannels::Application, "Failed to open file " );
		return false;
	}

	bool ImageResourceManager::ParseJsonFreeTexPacker( const nlohmann::json& json, const Filepath& filepath_prefix )
	{
		ASSERT( std::filesystem::is_directory( filepath_prefix ) );

		std::shared_ptr<Graphics::Texture> texture;
		size_t added_images = 0;

		const auto meta = json.at("meta");
		// load the texture
		{
			auto texture_filename = meta.at( "image" ).get<std::string>();
			if (texture_filename.empty())
				throw std::runtime_error( "No texture filename specified" );

			// TODO: use specified format information

			Graphics::TextureLoadProperties props;
			props.y_flip = false;
			texture = video.CreateTexture( filepath_prefix / texture_filename, props );
			if (!texture)
				throw std::runtime_error( "Failed to load texture" );
		}

		// parse images
		const auto frames = json.at( "frames" );
		for (const auto& [key, value] : frames.items())
		{
			const auto& image_id = key;

			const bool rotated = value.contains( "rotated" ) ? value["rotated"].get<bool>() : false;
			ASSERT( !rotated, "Currently don't support rotated images" );
			if (rotated)
				continue;

			const bool trimmed = value.contains( "trimmed" ) ? value["trimmed"].get<bool>() : false;
			ASSERT( !trimmed, "Currently don't support trimmed images" );
			if (trimmed)
				continue;

			const auto& frame = value.at( "frame" );
			ASSERT( frame.is_object(), "Expected frame to be an object" );
			if (!frame.is_object())
				continue;
			auto uvs = Rect<float> {
				Point2D<float>{ frame.at( "x" ).get<float>(), frame.at( "y" ).get<float>() },
				Size<float>{ frame.at( "w" ).get<float>(), frame.at( "h" ).get<float>() }
			};
			ASSERT( uvs.IsValid(), "Frame bounds are not a valid rectangle" );
			if (!uvs.IsValid())
				continue;

			AddImageDefinition( image_id, texture, uvs );
			++added_images;
		}
		ASSERT( added_images > 0, "Expected to load at least one image?" );

		return added_images > 0;
	}

	bool ImageResourceManager::AddImageDefinition( std::string_view image_id, std::shared_ptr<Graphics::Texture> texture, const Rect<float>& uvs )
	{
		ASSERT( !image_id.empty() );
		ASSERT( texture != nullptr );
		ASSERT( uvs.IsValid() );
		if (image_id.empty() || texture == nullptr || !uvs.IsValid())
			return false;

		auto def = std::make_shared<const ImageDefinition>( texture, uvs );
		auto [it, success] = id_to_image_def.try_emplace( (std::string)image_id, def );

		if (!success)
			LOG_WARN( LoggingChannels::Application, "Failed to add image definition to the resource manager, duplicate entry?" );

		return success;
	}
}
