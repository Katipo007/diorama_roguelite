#pragma once

class Core;
class ResourceManager;

namespace Resources
{
	enum class AssetType;
	class Resource;
	class BaseResourceCache;

	class ResourceLoader final
	{
	public:
		friend class BaseResourceCache;

		std::string_view GetAssetId() const noexcept { return id; }
		ResourceManager& GetManager() const noexcept { return manager; }

	private:
		ResourceLoader( std::string_view asset_id, ResourceManager& manager );
		virtual ~ResourceLoader();

		std::string id;
		ResourceManager& manager;
	};
}
