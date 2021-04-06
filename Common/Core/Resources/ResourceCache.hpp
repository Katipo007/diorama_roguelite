#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "ResourceTypes.hpp"

namespace Resources
{
	class Resource;
	class IResourceLoader;
	class ResourceManager;

	class BaseResourceCache
	{
	protected:
		using UntypedResourcePtr = std::shared_ptr<const Resource>;

	public:
		explicit BaseResourceCache( ResourceManager& manager, const AssetType type, std::unique_ptr<IResourceLoader> loader );
		virtual ~BaseResourceCache();

		AssetType GetResourceType() const { return type; }

		bool Exists( std::string_view resource_id ) const;
		UntypedResourcePtr GetUntyped( std::string_view resource_id ) const;

		void Unload( std::string_view resource_id );
		/// <summary>
		/// Unload resources that currently aren't being used.
		/// </summary>
		/// <param name="min_generations">Minimum number of generations this resource hasn't been used</param>
		void Purge( size_t min_generations = 3 );

		void NextGeneration();

	protected:
		void AddResource( std::unique_ptr<Resource>& new_resource );

	private:
		ResourceManager& manager;
		const AssetType type;
		std::unique_ptr<IResourceLoader> loader;

		struct ResourceEntry
		{
			size_t generation;
			UntypedResourcePtr resource;

			ResourceEntry( UntypedResourcePtr ptr, size_t generation )
				: resource( std::move( ptr ) ), generation( generation )
			{}
		};
		using ResourceHashmap_T = std::unordered_map<std::string, ResourceEntry>;

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
		ResourceCache( ResourceManager& manager, std::unique_ptr<IResourceLoader> loader )
			: BaseResourceCache( manager, std::move( loader ) )
		{}
		virtual ~ResourceCache() {}

		ResourcePtr Get( std::string_view resource_id ) const
		{
			return std::dynamic_pointer_cast<RESOURCE>(GetUntyped( resource_id ));
		}
	};
}
