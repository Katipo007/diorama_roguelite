#pragma once

#include "GameObjectId.hpp"
#include "GameObjectStore.hpp"
#include "ClientServerCommon/Game/Components/Concepts.hpp"
#include "Common/Utility/Entity/Fwd.hpp"

namespace Game
{
	class GameObjectStore;

	/// <summary>
	/// Provides an interface for entt entity ids and interacting with the gameworld
	/// </summary>
	class GameObject final
	{
		friend class GameObjectStore;

	public:
		~GameObject();

		GameObjectStore& Owner;
		GameObjectId_T GetId() const noexcept { return id; }

		bool IsNull() const noexcept;
		bool IsDeleted() const noexcept { return IsNull(); }

		template<Components::Component T> bool HasComponent() const noexcept { return Owner.HasComponent<T>( *this ); }
		template<Components::Component T> T* GetComponent() const noexcept { return Owner.GetComponent<T>( *this ); }

		template<Components::Component T, typename... Args> T& AddComponent( Args&&... args ) { return Owner.AddComponent<T>( *this, std::forward<Args>( args )... ); }
		template<Components::Component T> void RemoveComponent() { return Owner.RemoveComponent<T>( *this ); }

	private:
		GameObject( GameObjectStore& owner, entt::entity id );
		mutable GameObjectId_T id;
	};
}
