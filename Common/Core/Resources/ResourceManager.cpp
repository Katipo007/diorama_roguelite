#include "ResourceManager.hpp"

namespace Resources
{
	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager()
	{
		caches.clear();
	}

	BaseResourceCache* ResourceManager::GetCacheInternal( const AssetType type )
	{
		const auto it = caches.find( type );
		if (it != std::end( caches ))
			return it->second.get();

		return nullptr;
	}

	const BaseResourceCache* ResourceManager::GetCacheInternal( const AssetType type ) const
	{
		const auto it = caches.find( type );
		if (it != std::end( caches ))
			return it->second.get();

		return nullptr;
	}
}
