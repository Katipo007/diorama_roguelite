#pragma once

#include "ResourceHandle.hpp"
#include "ResourceCache.hpp"

namespace Resources
{
	enum class AssetType;
	class BaseResourceCache;
	class IResourceLoader;
	class Resource;

	class ResourceManager final
	{
		using CacheCollection_T = std::unordered_map<AssetType, std::unique_ptr<BaseResourceCache>>;

	public:
		explicit ResourceManager();
		virtual ~ResourceManager();

		template<typename RESOURCE>
		void Init( std::unique_ptr<IResourceLoader> loader )
		{
			static_assert(std::is_base_of<Resource, RESOURCE>::value, "Provided type must derive from Resources::Resource");
			constexpr AssetType type = RESOURCE::GetResourceType();

			if (const auto* existing_cache = GetCacheInternal( type ))
				throw std::runtime_error( "ResourceCache already initialised for this type of resource" );

			auto [it, success] = caches.try_emplace( type, std::make_unique<ResourceCache<RESOURCE>>( *this, std::move( loader ) ) );
			if (!success)
				throw std::runtime_error( "Failed to store ResourceCache of given type" );
		}

		template<typename RESOURCE>
		bool Exists( std::string_view resource_id ) const { return GetCache<RESOURCE>().Exists( resource_id ); }

		template<typename RESOURCE>
		bool IsLoaded( std::string_view resource_id ) const { return GetCache<RESOURCE>().IsLoaded( resource_id ); }

		template<typename RESOURCE>
		ResourceHandle<RESOURCE> Get( std::string_view resource_id ) const { return ResourceHandle<RESOURCE>( GetCache<RESOURCE>().Get( resource_id ) ); }

		template<typename RESOURCE>
		void Unload( std::string_view resource_id ) { GetCache<RESOURCE>().Unload( resource_id ); }

		template<typename RESOURCE>
		void Purge( size_t min_generations = 3 ) { GetCache<RESOURCE>().Purge( min_generations ); }

		template<typename RESOURCE>
		const ResourceCache<RESOURCE>& GetCache() const
		{
			static_assert(std::is_base_of<Resource, RESOURCE>::value, "Provided type must derive from Resources::Resource");
			if (const auto* cache = GetCacheInternal( RESOURCE::GetResourceType() ))
				return *dynamic_cast<ResourceCache<RESOURCE>*>(cache);

			throw std::runtime_error( "Given resource type is not initialised for this ResourceManager" );
		}

	protected:
		BaseResourceCache* GetCacheInternal( const AssetType type );
		const BaseResourceCache* GetCacheInternal( const AssetType type ) const;

	private:
		CacheCollection_T caches;
	};
}
