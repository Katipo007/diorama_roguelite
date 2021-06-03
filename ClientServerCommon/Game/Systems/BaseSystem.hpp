#pragma once

#include "SystemTypesEnum.hpp"
#include "Common/Utility/Entity/Fwd.hpp"
#include "Common/Utility/Timestep.hpp"
#include "Common/Utility/MagicEnum.hpp"

namespace Game::Systems
{
	class GameWorld;

	class BaseSystem
	{
		friend class GameWorld;

	public:
		virtual ~BaseSystem();

		SystemType GetType() const noexcept { return system_type; }

		virtual void OnFixedUpdate( entt::registry&, const Timestep& ) {}
		virtual void OnVariableUpdate( entt::registry&, const Timestep& ) {}

	protected:
		BaseSystem( SystemType type_id );

		virtual void OnAttached( GameWorld& ) {}
		virtual void OnDetached( GameWorld& ) {}

	private:
		const SystemType system_type;
	};
}
