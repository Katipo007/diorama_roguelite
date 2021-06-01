#pragma once

#include "Common/Core/Resources/ResourceHandle.hpp"
#include "Common/Core/Resources/ResourceCache.hpp"
#include "Common/Core/Resources/Concepts.hpp"

class Core;

namespace Resources
{
	enum class AssetType;
	class BaseResourceCache;
	class Resource;
}

class ResourceManager final
{
	using CacheCollection_T = std::unordered_map<Resources::AssetType, std::unique_ptr<Resources::BaseResourceCache>>;

public:
	explicit ResourceManager( ::Core& core );
	virtual ~ResourceManager();

	template<Resources::Concepts::Resource RESOURCE>
	void Init()
	{
		constexpr Resources::AssetType type = RESOURCE::GetResourceType();

		if (const auto* existing_cache = GetCacheInternal( type ))
			throw std::runtime_error( "ResourceCache already initialised for this type of resource" );

		auto [it, success] = caches.try_emplace( type, std::make_unique<Resources::ResourceCache<RESOURCE>>( *this ) );
		if (!success)
			throw std::runtime_error( "Failed to store ResourceCache of given type" );
	}

	template<Resources::Concepts::Resource RESOURCE>
	bool Exists( std::string_view resource_id ) const { return GetCache<RESOURCE>().Exists( resource_id ); }

	template<Resources::Concepts::Resource RESOURCE>
	bool IsLoaded( std::string_view resource_id ) const { return GetCache<RESOURCE>().IsLoaded( resource_id ); }

	template<Resources::Concepts::Resource RESOURCE>
	Resources::ResourceHandle<const RESOURCE> Get( std::string_view resource_id ) const { return Resources::ResourceHandle<const RESOURCE>( GetCache<RESOURCE>().Get( resource_id ) ); }

	template<Resources::Concepts::Resource RESOURCE>
	bool Preload( std::string_view resource_id ) const { return GetCache<RESOURCE>().Preload( resource_id ); }

	template<Resources::Concepts::Resource RESOURCE>
	void Unload( std::string_view resource_id ) { GetCache<RESOURCE>().Unload( resource_id ); }

	template<Resources::Concepts::Resource RESOURCE>
	void Purge( size_t min_generations = 3 ) { GetCache<RESOURCE>().Purge( min_generations ); }

	template<Resources::Concepts::Resource RESOURCE>
	[[nodiscard]] const Resources::ResourceCache<RESOURCE>& GetCache() const
	{
		constexpr Resources::AssetType type = RESOURCE::GetResourceType();
		if (const auto* cache = GetCacheInternal( type ))
			return *dynamic_cast<const Resources::ResourceCache<RESOURCE>*>(cache);

		throw std::runtime_error( "Given resource type is not initialised for this ResourceManager" );
	}

	Core& GetCore() noexcept { return core; }
	const Core& GetCore() const noexcept { return core; }

protected:
	Resources::BaseResourceCache* GetCacheInternal( const Resources::AssetType type );
	const Resources::BaseResourceCache* GetCacheInternal( const Resources::AssetType type ) const;

private:
	CacheCollection_T caches;
	Core& core;
};
