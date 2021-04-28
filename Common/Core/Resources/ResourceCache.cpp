#include "ResourceCache.hpp"

#include <algorithm>
#include <execution>

#include "Resource.hpp"
#include "ResourceLoader.hpp"

namespace Resources
{
	BaseResourceCache::BaseResourceCache( ResourceManager& _manager, const AssetType _type )
		: manager( _manager )
		, type( _type )
	{
	}

	BaseResourceCache::~BaseResourceCache()
	{
		resources.clear();
	}

	bool BaseResourceCache::Exists( std::string_view resource_id_ ) const
	{
		const auto it = resources.find( resource_id_ );
		if (it != std::end( resources ))
			return true;

		return false;
	}

	BaseResourceCache::UntypedResourcePtr BaseResourceCache::GetUntyped( std::string_view resource_id_ ) const
	{
		const auto it = resources.find( resource_id_ );
		if (it != std::end( resources ))
			return it->second.resource;

		auto* non_const_this = const_cast<BaseResourceCache*>(this);
		UntypedResourcePtr result;

		// not in the cache, try loading
		auto loader = ResourceLoader( resource_id_, non_const_this->manager );
		auto loaded_resource = LoadResource( loader );
		if (loaded_resource)
		{
			loaded_resource->resource_id = std::string{ resource_id_ };
			ASSERT( !loaded_resource->GetResourceId().empty() );
			result = non_const_this->AddResource( loaded_resource );
		}

		if constexpr (std::is_same<UntypedResourcePtr, std::unique_ptr<const Resource>>::value)
			ASSERT( !loaded_resource ); // unique_ptr should have been moved
		
		return result;
	}

	bool BaseResourceCache::Preload( std::string_view resource_id_ ) const
	{
		auto result = GetUntyped( resource_id_ );
		if (!result)
			LOG_WARN( Application, "Preloading resource '{}' (type: {}) failed", resource_id_, GetAssetTypeName( type ) );

		return result != nullptr;
	}

	void BaseResourceCache::Unload( std::string_view resource_id_ )
	{
		const auto it = resources.find( resource_id_ );
		if (it != std::end( resources ))
			resources.erase( it );
	}

	void BaseResourceCache::Purge( size_t _min_generations )
	{
		const auto generation_threshold = std::min( current_generation, current_generation - _min_generations );

		const auto old_size = resources.size();
		for (auto it = std::begin( resources ), last = std::end( resources ); it != last; )
		{
			if (it->second.generation < generation_threshold)
				it = resources.erase( it );
			else
				++it;
		}
		
		LOG_INFO( Application, "Purged '{}' old resources from {} cache", old_size - resources.size(), GetAssetTypeName( type ) );
	}

	void BaseResourceCache::NextGeneration()
	{
		std::for_each( std::execution::par_unseq, std::begin( resources ), std::end( resources ), [ g=this->current_generation ]( ResourceHashmap_T::value_type& entry )
			{
				if (entry.second.resource.use_count() > 1)
					entry.second.generation = g;
			} );
	}

	BaseResourceCache::UntypedResourcePtr BaseResourceCache::AddResource( UntypedResourcePtr& new_resource )
	{
		ASSERT( new_resource );
		if (!new_resource)
			return nullptr;

		const auto resource_id = std::string{ new_resource->GetResourceId() };
		ASSERT( !resource_id.empty() );
		if (resource_id.empty())
			return nullptr;

		auto [it, success] = resources.try_emplace( resource_id, new_resource, current_generation );
		if (success)
			return it->second.resource;

		ASSERT( false, "Failed to add resource, does a resource with the given id already exist?" );
		return nullptr;
	}

	BaseResourceCache::UntypedResourcePtr BaseResourceCache::AddResource( UntypedNonConstResourcePtr& new_resource )
	{
		ASSERT( new_resource );
		if (!new_resource)
			return nullptr;

		const auto resource_id = std::string{ new_resource->GetResourceId() };
		ASSERT( !resource_id.empty() );
		if (resource_id.empty())
			return nullptr;

		auto [it, success] = resources.try_emplace( resource_id, new_resource, current_generation );
		if (success)
			return it->second.resource;

		ASSERT( false, "Failed to add resource, does a resource with the given id already exist?" );
		return nullptr;
	}

	BaseResourceCache::UntypedResourcePtr BaseResourceCache::FindIf( const std::function<bool( const Resource& )>& pred ) const
	{
		const auto pred_wrapper = [&]( const ResourceHashmap_T::value_type& entry ) -> bool
		{
			return pred( *entry.second.resource );
		};
		auto it = std::find_if( resources.begin(), resources.end(), pred_wrapper );
		if (it != std::end( resources ))
			return it->second.resource;

		return nullptr;
	}
}
