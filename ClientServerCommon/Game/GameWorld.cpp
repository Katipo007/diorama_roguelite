#include "GameWorld.hpp"

#include "ClientServerCommon/Game/Objects/GameObjectStore.hpp"
#include "ClientServerCommon/Game/Systems/BaseSystem.hpp"
#include "Common/Utility/Containers.hpp"

namespace Game
{
	struct GameWorld::Pimpl
	{
		GameObjectStore gameobject_store;

		Pimpl( GameWorld& owner )
			: gameobject_store( owner )
		{
		}
	};


	GameWorld::GameWorld()
		: pimpl( new Pimpl( *this ) )
	{
	}

	GameWorld::~GameWorld() = default;

	void GameWorld::OnFixedUpdate( const PreciseTimestep& timestep )
	{
		for (auto& system : systems)
			system->OnFixedUpdate( GetObjectStore(), timestep );
	}

	void GameWorld::OnVariableUpdate( const PreciseTimestep& timestep )
	{
		for (auto& system : systems)
			system->OnVariableUpdate( GetObjectStore(), timestep );
	}

	GameObjectStore& GameWorld::GetObjectStore() noexcept
	{
		return pimpl->gameobject_store;
	}

	const GameObjectStore& GameWorld::GetObjectStore() const noexcept
	{
		return pimpl->gameobject_store;
	}

	Systems::BaseSystem* GameWorld::GetSystem( Systems::SystemType type )
	{
		const auto it = FindIf( systems, [type]( const std::unique_ptr<Systems::BaseSystem>& entry ) -> bool { return !!entry && entry->GetType() == type; } );
		if (it != std::end( systems ))
			return it->get();

		return nullptr;
	}

	void GameWorld::AddSystemInternal( std::unique_ptr<Systems::BaseSystem> new_system )
	{
		if (GetSystem( new_system->GetType() ) != nullptr)
			throw std::runtime_error{ "System of given type already exists for this game world" };

		systems.emplace_back( std::move( new_system ) );
	}

	void GameWorld::RemoveSystemInternal( Systems::BaseSystem& to_remove )
	{
		const auto it = FindIf( systems, [to_remove]( const std::unique_ptr<Systems::BaseSystem>& entry ) -> bool { return !!entry && (entry.get() != &to_remove); } );
		if (it == std::end( systems ))
		{
			ASSERT( false, "Tried to remove a system not present in this game world" );
			return;
		}

		systems.erase( it );
	}
}
