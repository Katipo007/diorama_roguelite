#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Common/Utility/StringHashMap.hpp"
#include "ResourceTypes.hpp"

class ResourceManager;

namespace Resources
{
	class Resource;
	class ResourceLoader;

	class BaseResourceCache
	{
	protected:
		using UntypedResourcePtr = std::shared_ptr<const Resource>;
		using UntypedNonConstResourcePtr = std::shared_ptr<Resource>;

	public:
		explicit BaseResourceCache( ResourceManager& manager, const AssetType type );
		virtual ~BaseResourceCache();

		AssetType GetResourceType() const { return type; }

		[[nodiscard]] bool Exists( std::string_view resource_id ) const;
		[[nodiscard]] UntypedResourcePtr GetUntyped( std::string_view resource_id ) const;

		bool Preload( std::string_view resource_id ) const;
		void Unload( std::string_view resource_id );
		/// <summary>
		/// Unload resources that currently aren't being used.
		/// </summary>
		/// <param name="min_generations">Minimum number of generations this resource hasn't been used</param>
		void Purge( size_t min_generations = 3 );

		void NextGeneration();

	protected:
		UntypedResourcePtr AddResource( UntypedResourcePtr& new_resource );
		UntypedResourcePtr AddResource( UntypedNonConstResourcePtr& new_resource );
		[[nodiscard]] virtual UntypedNonConstResourcePtr LoadResource( ResourceLoader& loader ) const = 0;

		[[nodiscard]] UntypedResourcePtr FindIf( const std::function<bool( const Resource& )>& pred ) const;

	private:
		ResourceManager& manager;
		const AssetType type;

		struct ResourceEntry
		{
			size_t generation;
			UntypedResourcePtr resource;

			ResourceEntry( UntypedResourcePtr ptr, size_t generation )
				: resource( std::move( ptr ) ), generation( generation )
			{}
		};
		using ResourceHashmap_T = StringHashMap<ResourceEntry>;

		ResourceHashmap_T resources;
		size_t current_generation = 0;
	};

	template<typename RESOURCE>
	class ResourceCache
		: public BaseResourceCache
	{
	protected:
		using ResourcePtr = std::shared_ptr<const RESOURCE>;

	public:
		ResourceCache( ResourceManager& manager )
			: BaseResourceCache( manager, RESOURCE::GetResourceType() )
		{}
		virtual ~ResourceCache() {}

		[[nodiscard]] ResourcePtr Get( std::string_view resource_id ) const
		{
			return std::dynamic_pointer_cast<const RESOURCE>(GetUntyped( resource_id ));
		}

		[[nodiscard]] ResourcePtr FindIf( const std::function<bool( const RESOURCE& )>& pred ) const
		{
			const auto predicate_wrapper = [&pred]( const Resource& entry ) -> bool
			{
				return pred( dynamic_cast<const RESOURCE&>( entry ) );
			};
			
			if (const auto it = BaseResourceCache::FindIf( predicate_wrapper ))
				return std::dynamic_pointer_cast<const RESOURCE>(it);

			return nullptr;
		}

		[[nodiscard]] virtual UntypedNonConstResourcePtr LoadResource( ResourceLoader& loader ) const override
		{
			auto loaded_resource = RESOURCE::LoadResource( loader );
			static_assert(std::is_same<decltype(loaded_resource), std::shared_ptr<RESOURCE>>::value, "RESOURCE::LoadResource() returns the wrong type, expected a std::shared_ptr<const RESOURCE>");
			return std::dynamic_pointer_cast<Resource>( loaded_resource );
		}
	};
}
