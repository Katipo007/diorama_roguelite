#include "BaseSystem.hpp"

namespace Game::Systems
{
	BaseSystem::BaseSystem( SystemType type_id )
		: system_type( type_id )
	{
		ASSERT( system_type != SystemType::Unknown );
	}

	BaseSystem::~BaseSystem() = default;
}
