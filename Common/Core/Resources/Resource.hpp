#pragma once

#include <string>
#include <limits>

namespace Resources
{
	enum class AssetType;
	class IResourceLoader;

	// baseclass for resources
	class Resource
	{
		friend class IResourceLoader; // to set the resource id
		friend class BaseResourceCache; // to handle generations

	public:
		virtual ~Resource() {}
		
		std::string_view GetResourceId() const { return resource_id; }
	
	protected:
		Resource() {}

	private:
		std::string resource_id;
	};
}
