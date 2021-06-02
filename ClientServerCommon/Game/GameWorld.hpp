#pragma once

#include "Common/Utility/Entity/EnttHeader.hpp"
#include "Common/Utility/Timestep.hpp"
#include "Common/Utility/NonCopyable.hpp"

#include "ClientServerCommon/Game/Systems/Concepts.hpp"
#include "ClientServerCommon/Game/Systems/SystemTypesEnum.hpp"

namespace Game::Systems { class BaseSystem; }

namespace Game
{
	// Base type for ClientWorld and ServerWorld
	class GameWorld
		: NonCopyable
	{
	public:
		virtual ~GameWorld();

		virtual void OnFixedUpdate( const PreciseTimestep& );
		virtual void OnVariableUpdate( const PreciseTimestep& );

		Systems::BaseSystem* GetSystem( Systems::SystemType type );
		template<Systems::System T>
		T* GetSystem()
		{
			constexpr Systems::SystemType type_id = T::GetStaticType();
			static_assert(type_id != Systems::SystemType::Unknown);

			if (auto* system = GetSystem( type_id ))
				return dynamic_cast<T*>(system);

			return nullptr;
		}

	protected:
		void AddSystemInternal( std::unique_ptr<Systems::BaseSystem> );
		void RemoveSystemInternal( Systems::BaseSystem& );

	protected:
		GameWorld();

		std::vector<std::unique_ptr<Systems::BaseSystem>> systems;
		entt::registry entity_registry;
	};
}
