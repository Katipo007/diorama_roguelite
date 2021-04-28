#pragma once

#pragma push_macro("X")
#undef X

#pragma push_macro("ASSET_TYPES")
#define ASSET_TYPES \
	X( Texture ) \
	X( Shader ) \
	X( SpriteSheet ) \
	X( Sprite )

namespace Resources
{
#define X( type ) type,
	enum class AssetType
	{
		ASSET_TYPES
	};
#undef X

	constexpr const char* GetAssetTypeName( const AssetType type )
	{
#define X( type ) #type,
		constexpr const char* const names[] = { ASSET_TYPES };
#undef X

		return names[(size_t)type];
	}
}

#pragma pop_macro("ASSET_TYPES")
#pragma pop_macro("X")
