#pragma once

#include "SystemTypesEnum.hpp"
#include "Common/Utility/Entity/Fwd.hpp"
#include "Common/Utility/Timestep.hpp"
#include "Common/Utility/MagicEnum.hpp"

namespace Game::Systems
{
	class BaseSystem
	{
	public:
		virtual ~BaseSystem();

		SystemType GetType() const noexcept { return system_type; }

		virtual void OnFixedUpdate( entt::registry&, const Timestep& ) {}
		virtual void OnVariableUpdate( entt::registry&, const Timestep& ) {}

	protected:
		BaseSystem( SystemType type_id );

	private:
		const SystemType system_type;
	};
}
