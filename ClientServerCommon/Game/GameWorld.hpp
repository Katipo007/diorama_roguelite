#pragma once

#include "ClientServerCommon/Game/Systems/Concepts.hpp"
#include "ClientServerCommon/Game/Systems/SystemTypesEnum.hpp"

#include "Common/Utility/Timestep.hpp"
#include "Common/Utility/NonCopyable.hpp"

namespace Game::Systems { class BaseSystem; }

namespace Game
{
	class GameObject;
	class GameObjectStore;

	// Base type for ClientWorld and ServerWorld
	class GameWorld
		: NonCopyable
	{
	public:
		virtual ~GameWorld();

		virtual void OnFixedUpdate( const PreciseTimestep& );
		virtual void OnVariableUpdate( const PreciseTimestep& );

		GameObjectStore& GetObjectStore() noexcept;
		const GameObjectStore& GetObjectStore() const noexcept;

#pragma region Systems
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
#pragma endregion Systems

	protected:
		GameWorld();

		void AddSystemInternal( std::unique_ptr<Systems::BaseSystem> );
		void RemoveSystemInternal( Systems::BaseSystem& );

	private:
		std::vector<std::unique_ptr<Systems::BaseSystem>> systems;

		struct Pimpl;
		const std::unique_ptr<Pimpl> pimpl;
	};
}
