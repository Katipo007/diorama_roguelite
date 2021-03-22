#pragma once

#include "Common/Utility/Timestep.hpp"

namespace entt
{
	class registry;
}

namespace Game::Systems
{
	class AbstractSystem
	{
	public:
		virtual ~AbstractSystem() = default;

		virtual void Update( entt::registry&, const Timestep& ) {}
		
		// TODO: have a Draw() method??
	};
}
 