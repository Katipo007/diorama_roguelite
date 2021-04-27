#include "ResourceCache.hpp"

#include <algorithm>
#include <execution>

#include "Resource.hpp"
#include "ResourceLoader.hpp"

namespace
{
	thread_local static std::string temp_str;
}

#define TEMP_STR_FROM_VIEW( str_view ) { temp_str.reserve( std::max( temp_str.capacity(), str_view.size() ) ); temp_str.assign( std::begin( str_view ), std::end( str_view ) ); }

#ifdef ENABLE_ASSERTS
#define TEMP_STR_UNCHANGED_TEST( str_view ) { ASSERT( temp_str == str_view ); }
#else
#define TEMP_STR_UNCHANGED_TEST
#endif

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

	bool BaseResourceCache::Exists( std::string_view _resource_id ) const
	{
		TEMP_STR_FROM_VIEW( _resource_id );
		const auto it = resources.find( temp_str );
		if (it != std::end( resources ))
			return true;

		return false;
	}

	BaseResourceCache::UntypedResourcePtr BaseResourceCache::GetUntyped( std::string_view _resource_id ) const
	{
		TEMP_STR_FROM_VIEW( _resource_id );

		const auto it = resources.find( temp_str );
		if (it != std::end( resources ))
			return it->second.resource;

		auto* non_const_this = const_cast<BaseResourceCache*>(this);

		// not in the cache, try loading
		auto loader = ResourceLoader( _resource_id, non_const_this->manager );
		auto loaded_resource = LoadResource( loader );
		if (loaded_resource)
		{
			ASSERT( !loaded_resource->GetResourceId().empty() );
			non_const_this->AddResource( loaded_resource );
		}

		ASSERT( !loaded_resource ); // unique_ptr should have been moved

		return UntypedResourcePtr();
	}

	void BaseResourceCache::Unload( std::string_view _resource_id )
	{
		TEMP_STR_FROM_VIEW( _resource_id );
		resources.erase( temp_str );
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

	void BaseResourceCache::AddResource( UntypedResourcePtr& _new_resource )
	{
		ASSERT( _new_resource );
		if (!_new_resource)
			return;

		const auto resource_id = _new_resource->GetResourceId();
		ASSERT( !resource_id.empty() );
		TEMP_STR_FROM_VIEW( resource_id );

		auto [it, success] = resources.try_emplace( temp_str, _new_resource, current_generation );
		ASSERT( success );
	}
}
