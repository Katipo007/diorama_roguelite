#pragma once

#include <string>
#include <limits>

namespace Resources
{
	enum class AssetType;

	// baseclass for resources
	class Resource
	{
		friend class BaseResourceCache; // to handle generations

	public:
		virtual ~Resource() = default;
		
		std::string_view GetResourceId() const noexcept { return resource_id; }
	
	protected:
		Resource() = default;

	private:
		std::string resource_id;
	};
}
