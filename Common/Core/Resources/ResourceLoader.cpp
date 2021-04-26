#include "ResourceLoader.hpp"
#include "ResourceLoader.hpp"

namespace Resources
{
	ResourceLoader::ResourceLoader( std::string_view asset_id_, ResourceManager& manager_ )
		: id( asset_id_ )
		, manager( manager_ )
	{
	}

	ResourceLoader::~ResourceLoader()
	{
	}
}
