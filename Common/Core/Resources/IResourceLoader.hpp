#pragma once

class ResourceManager;

namespace Resources
{
	enum class AssetType;
	class Resource;

	class IResourceLoader
	{
	public:
		virtual ~IResourceLoader() {}
		/// <summary>
		/// Called by the ResourceCache when it is constructed
		/// </summary>
		/// <param name="manager"></param>
		virtual void Init( ResourceManager& manager ) = 0;

		/// <summary>
		/// Load a given asset
		/// </summary>
		/// <param name="asset_id">ID of the resource/asset to load</param>
		/// <param name="type">Type of asset we are expecting</param>
		/// <returns>The resource</returns>
		virtual std::unique_ptr<Resource> LoadAsset( std::string_view asset_id, const AssetType type ) const = 0;
	};
}
