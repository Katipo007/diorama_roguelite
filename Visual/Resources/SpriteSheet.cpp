#include "SpriteSheet.hpp"

#include "Common/Core/ResourceManager.hpp"
#include "Common/Core/Resources/ResourceLoader.hpp"
#include "Common/File/FileOps.hpp"
#include "Common/Utility/Containers.hpp"
#include "Common/Utility/json.hpp"

#include "Visual/Graphics/Texture.hpp"

#ifdef _DEBUG
#	define DEBUG_SPRITESHEET_LOADING 1
#endif

namespace
{
	bool ParseFreeTexPackerSpritesheetJson( const nlohmann::json& json, const Filepath& filepath_prefix, std::vector<std::pair<std::string, Graphics::SpriteSheetEntry>>& out, Filepath& texture_filename_out )
	{
		ASSERT( std::filesystem::is_directory( filepath_prefix ) );

		const size_t old_out_size = out.size();

		const auto parse_frame = [&]( std::string_view key, const nlohmann::json& obj, const Size<float>& texture_size )
		{
			// rotated
			const bool rotated = obj.contains( "rotated" ) ? obj["rotated"].get<bool>() : false;
			if (rotated)
			{
				ASSERT( false, "Currently don't support rotated images" );
				return;
			}

			// trimmed
			const bool trimmed = obj.contains( "trimmed" ) ? obj["trimmed"].get<bool>() : false;
			if (trimmed)
			{
				ASSERT( false, "Currently don't support trimmed images" );
				return;
			}

			// frame
			const auto& frame = obj.at( "frame" );
			ASSERT( frame.is_object(), "Expected frame to be an object" );
			const auto rect = Rect<float>{
				Point2D<float>{ frame["x"].get<float>(), frame["y"].get<float>() },
				Size<float>{ frame["w"].get<float>(), frame["h"].get<float>() }
			};
			ASSERT( rect.IsValid(), "Frame bounds are not a valid rectangle" );
			if (!rect.IsValid())
				return;

			// pivot
			auto pivot = Point2D<float>{ 0, 0 };
			if (obj.contains( "pivot" ))
			{
				const auto& pivot_obj = obj.at( "pivot" );
				pivot = { pivot_obj["x"].get<float>(), pivot_obj["y"].get<float>() };
			}

			Graphics::SpriteSheetEntry entry;
			entry.size = { rect.w, rect.h };
			entry.pivot = { entry.size.width * pivot.x, entry.size.height * pivot.y };
			entry.uvs = { Point2D<float>{ rect.x / texture_size.width, rect.y / texture_size.height }, Size<float>{ rect.w / texture_size.width, rect.h / texture_size.height } };
			entry.rotated = rotated;
			entry.trimmed = trimmed;
			out.emplace_back( key, std::move( entry ) );
		};

		Size<float> texture_size;
		const auto meta = json.at( "meta" );
		// id the texture
		{
			auto texture_filename = meta.at( "image" ).get<std::string>();
			if (!texture_filename.empty())
				texture_filename_out = filepath_prefix / texture_filename;
			else
			{
				LOG_WARN( LoggingChannels::Application, "Spritesheet JSON didn't supply a texture filename" );
				texture_filename_out = "";
			}

			const auto& size_obj = meta.at( "size" );
			texture_size = { size_obj.at( "w" ).get<float>(), size_obj.at( "h" ).get<float>() };
			ASSERT( texture_size.width > 0 && texture_size.height > 0 );
		}

		// parse images
		const auto frames = json.at( "frames" );
		for (const auto& [key, value] : frames.items())
		{
			try
			{
				parse_frame( key, value, texture_size );
			}
			catch( std::runtime_error& )
			{
#if DEBUG_SPRITESHEET_LOADING
				ASSERT( false );
#endif
				continue;
			}
		}

		const auto added_images = out.size() - old_out_size;
		ASSERT( added_images > 0, "Expected to load at least one image?" );
		return added_images > 0;
	}
}

namespace Graphics
{
	const std::shared_ptr<const Texture>& SpriteSheet::GetTexture() const noexcept
	{
		if (!texture)
			ReloadTexture( manager );

		return texture;
	}

	const SpriteSheetEntry& SpriteSheet::GetSprite( std::string_view name ) const
	{
		return GetSprite( GetSpriteIndex( name ) );
	}

	const SpriteSheetEntry& SpriteSheet::GetSprite( SpriteIdx_T idx ) const
	{
		return sprites.at( idx );
	}

	size_t SpriteSheet::GetNumSprites() const noexcept
	{
		return std::size( sprites );
	}

	SpriteSheet::SpriteIdx_T SpriteSheet::GetSpriteIndex( std::string_view name ) const
	{
		const auto it = id_to_sprite_idx_map.find( name );
		if (it != std::end( id_to_sprite_idx_map ))
			return it->second;
		
		LOG_ERROR( LoggingChannels::Application, "SpriteSheet does not contain sprite '{}'", name );
		return static_cast<SpriteIdx_T>(-1);
	}

	bool SpriteSheet::HasSprite( std::string_view name ) const
	{
		return id_to_sprite_idx_map.contains( name );
	}

	bool SpriteSheet::LoadFromJson( std::string_view json_string, const Filepath& filepath_prefix )
	{
		ASSERT( std::filesystem::is_directory( filepath_prefix ) );
		auto j = nlohmann::json::parse( json_string );
		bool handled = false;

		Filepath texture_filename;
		std::vector<std::pair<std::string, SpriteSheetEntry>> entries;
		try
		{
			if (j.contains( "frames" ) && j.contains( "meta" ) && j["meta"]["app"].is_string() && j["meta"]["app"].get<std::string>() == "http://free-tex-packer.com")
				handled = ParseFreeTexPackerSpritesheetJson( j, filepath_prefix, entries, texture_filename );
		}
		catch (nlohmann::json::exception& e)
		{
			LOG_ERROR( LoggingChannels::Application, "Exception while parsing spritesheet json: '{}'", e.what() );
			handled = false;
		}
		catch (std::runtime_error& e)
		{
			LOG_ERROR( LoggingChannels::Application, "Exception while parsing spritesheet:'{}'", e.what() );
			handled = false;
		}

		texture_id = Filepath( texture_filename ).lexically_normal().generic_string();
		ASSERT( !texture_id.empty() );
		for (auto& entry : entries)
			AddSprite( entry.first, entry.second );

		return handled;
	}

	void SpriteSheet::SetTextureId( std::string_view new_texture_id )
	{
		this->texture_id = new_texture_id;
		if (texture)
			ReloadTexture( manager );
	}

	void SpriteSheet::AddSprite( std::string_view name, const SpriteSheetEntry& sprite )
	{
		const auto [it, success] = id_to_sprite_idx_map.try_emplace( std::string{ name }, static_cast<SpriteIdx_T>(sprites.size()) );
		if( success )
			sprites.push_back( sprite );
	}

	std::shared_ptr<SpriteSheet> SpriteSheet::LoadResource( ::Resources::ResourceLoader& loader )
	{
		using namespace std::string_literals;

		auto sheet = std::make_shared<SpriteSheet>( loader.GetManager() );
		
		const Filepath filepath = loader.GetAssetId();
		std::string file_contents;
		if (FileOps::ReadFile( filepath, file_contents ))
			sheet->LoadFromJson( file_contents, FileOps::GetFileDirectory( filepath ) );
		else
			throw std::runtime_error( "Failed to load spritesheet '"s + std::string( loader.GetAssetId() ) + "'"s );

		return sheet;
	}

	void SpriteSheet::ReloadTexture( ResourceManager& manager_ ) const
	{
		texture = manager_.Get<Texture>( texture_id );
	}


	/// 
	/// Sprite
	/// 

	Sprite::Sprite( std::shared_ptr<const SpriteSheet> parent_, SpriteSheet::SpriteIdx_T idx_ )
		: parent_spritesheet( parent_ )
		, index( idx_ )
	{
	}

	const SpriteSheetEntry& Sprite::GetSprite() const
	{
		if( auto sheet = GetSpriteSheet() )
			return sheet->GetSprite( index );

		throw std::runtime_error( "Spritesheet has expired" );
	}

	SpriteSheet::SpriteIdx_T Sprite::GetIndex() const noexcept
	{
		return index;
	}

	std::shared_ptr<const SpriteSheet> Sprite::GetSpriteSheet() const
	{
		return parent_spritesheet.lock();
	}

	std::shared_ptr<Sprite> Sprite::LoadResource( ::Resources::ResourceLoader& loader )
	{
		auto& manager = loader.GetManager();
		auto& spritesheets = manager.GetCache<SpriteSheet>();
		const auto the_sheet = spritesheets.FindIf( [&loader]( const SpriteSheet& entry ) -> bool
			{
				return entry.HasSprite( loader.GetAssetId() );
			} );

		if (the_sheet)
			return std::make_shared<Sprite>( the_sheet, the_sheet->GetSpriteIndex( loader.GetAssetId() ) );

		return nullptr;
	}
}
