#pragma once

#include "Common/Core/Resources/Resource.hpp"
#include "Common/Core/Resources/ResourceTypes.hpp"

#include "Common/File/Filepath.hpp"
#include "Common/Geometry/Point2D.hpp"
#include "Common/Geometry/Size.hpp"
#include "Common/Geometry/Rect.hpp"
#include "Common/Utility/StringHashMap.hpp"

class ResourceManager;

namespace Resources { class ResourceLoader; }

namespace Graphics
{
	class Texture;

	struct SpriteSheetEntry
	{
		Point2D<float> pivot;
		Rect<float> uvs;
		Size<float> size;
		bool rotated = false;
		bool trimmed = false;
	};

	class SpriteSheet
		: public ::Resources::Resource
	{
	public:
		using SpriteIdx_T = uint32_t;

		SpriteSheet( ResourceManager& manager_ ) : manager( manager_ ) {}

		const std::shared_ptr<const Texture>& GetTexture() const noexcept;
		const SpriteSheetEntry& GetSprite( std::string_view name ) const;
		const SpriteSheetEntry& GetSprite( SpriteIdx_T idx ) const;

		size_t GetNumSprites() const noexcept;
		SpriteIdx_T GetSpriteIndex( std::string_view name ) const;
		bool HasSprite( std::string_view name ) const;

		bool LoadFromJson( std::string_view json_string, const Filepath& filepath_prefix );
		void SetTextureId( std::string_view new_texture_id );
		void AddSprite( std::string_view name, const SpriteSheetEntry& sprite );

		static constexpr ::Resources::AssetType GetResourceType() noexcept { return ::Resources::AssetType::SpriteSheet; }
		static std::shared_ptr<SpriteSheet> LoadResource( ::Resources::ResourceLoader& loader );

	protected:
		void ReloadTexture( ResourceManager& manager ) const;

	protected:
		ResourceManager& manager;

		std::string texture_id;
		mutable std::shared_ptr<const Texture> texture;
		std::vector<SpriteSheetEntry> sprites;
		StringHashMap<SpriteIdx_T> id_to_sprite_idx_map;
	};

	class Sprite
		: public ::Resources::Resource
	{
	public:
		Sprite( std::shared_ptr<const SpriteSheet> parent, SpriteSheet::SpriteIdx_T idx );

		const SpriteSheetEntry& GetSprite() const;
		SpriteSheet::SpriteIdx_T GetIndex() const noexcept;
		std::shared_ptr<const SpriteSheet> GetSpriteSheet() const;

		static constexpr ::Resources::AssetType GetResourceType() noexcept { return ::Resources::AssetType::Sprite; }
		static std::shared_ptr<Sprite> LoadResource( ::Resources::ResourceLoader& loader );

	protected:
		std::weak_ptr<const SpriteSheet> parent_spritesheet;
		SpriteSheet::SpriteIdx_T index = static_cast<SpriteSheet::SpriteIdx_T>(-1);
	};
}
