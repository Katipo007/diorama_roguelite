#pragma once

#include "SystemTypesEnum.hpp"
#include "Common/Utility/Timestep.hpp"
#include "Common/Utility/MagicEnum.hpp"

namespace Game { class GameObjectStore; }

namespace Game::Systems
{
	class GameWorld;

	class BaseSystem
	{
		friend class GameWorld;

	public:
		virtual ~BaseSystem();

		SystemType GetType() const noexcept { return system_type; }

		virtual void OnFixedUpdate( Game::GameObjectStore&, const Timestep& ) {}
		virtual void OnVariableUpdate( Game::GameObjectStore&, const Timestep& ) {}

	protected:
		BaseSystem( SystemType type_id );

		virtual void OnAttached( GameWorld& ) {}
		virtual void OnDetached( GameWorld& ) {}

	private:
		const SystemType system_type;
	};
}
