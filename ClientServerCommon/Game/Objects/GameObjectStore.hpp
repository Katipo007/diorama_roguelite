#pragma once

#include "ClientServerCommon/Game/Objects/GameObjectId.hpp"
#include "ClientServerCommon/Game/Objects/GameObjectPtr.hpp"
#include "ClientServerCommon/Game/Components/Concepts.hpp"
#include "Common/Utility/Entity/EnttHeader.hpp"
#include "Common/Utility/Signal.hpp"
#include "Common/Utility/NonCopyable.hpp"

namespace Game
{
	class GameWorld;
	class GameObject;

	class GameObjectStore final
		: NonCopyable
	{
		friend class GameWorld;

	public:
		~GameObjectStore();

		GameWorld& World;

		bool Exists( const GameObject& object ) const noexcept { return Exists( GetInternalId( object ) ); }
		bool Exists( GameObjectId_T ) const noexcept;

		GameObjectPtr FindObject( GameObjectId_T ) noexcept;

		template<Components::Component T>
		inline bool HasComponent( const GameObject& object ) const noexcept { return GetComponent<T>() != nullptr; }

		template<Components::Component T>
		T* GetComponent( GameObject& object ) const noexcept
		{
			return registry.try_get<T>( GetInternalId( object ) );
		}

		template<Components::Component T, typename... Args>
		T& AddComponent( GameObject& object, Args&&... args )
		{
			return registry.emplace<T>( GetInternalId( object ), std::forward<Args>( args )... );
		}

		template<Components::Component T>
		void RemoveComponent( GameObject& object )
		{
			return registry.remove_if_exists<T>( GetInternalId( object ) );
		}

		GameObjectPtr CreateObject();
		void DestroyObject( GameObject& );


		Signal::signal<GameObject&> ObjectCreated;
		Signal::signal<GameObject&> ObjectDestroyed;

	protected: // intended to be used by friends
		GameObjectStore( GameWorld& owner );

		entt::entity GetInternalId( const GameObject& ) const noexcept;
		GameObjectPtr GetObjectPtr( entt::entity ) noexcept;

	private:
		entt::registry registry;
		std::unordered_map<entt::entity, GameObjectPtr> cached_object_pointers;
	};
}
