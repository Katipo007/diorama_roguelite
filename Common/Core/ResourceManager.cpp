#include "ResourceManager.hpp"

ResourceManager::ResourceManager( ::Core& core_ )
	: core( core_ )
{
}

ResourceManager::~ResourceManager()
{
	caches.clear();
}

Resources::BaseResourceCache* ResourceManager::GetCacheInternal( const Resources::AssetType type )
{
	const auto it = caches.find( type );
	if (it != std::end( caches ))
		return it->second.get();

	return nullptr;
}

const Resources::BaseResourceCache* ResourceManager::GetCacheInternal( const Resources::AssetType type ) const
{
	const auto it = caches.find( type );
	if (it != std::end( caches ))
		return it->second.get();

	return nullptr;
}
