#include "GameObjectStore.hpp"

#include "GameObject.hpp"

namespace Game
{
	GameObjectStore::GameObjectStore( GameWorld& owner )
		: World( owner )
	{
	}

	GameObjectStore::~GameObjectStore()
	{
	}

	bool GameObjectStore::Exists( GameObjectId_T id ) const noexcept
	{
		return registry.valid( id );
	}

	GameObjectPtr GameObjectStore::FindObject( GameObjectId_T id ) noexcept
	{
		if( !registry.valid( id ) )
			return {};

		return GetObjectPtr( id );
	}

	entt::entity GameObjectStore::GetInternalId( const GameObject& object ) const noexcept
	{
		if (&object.Owner == this)
			return object.GetId();

		return entt::null;
	}

	GameObjectPtr GameObjectStore::GetObjectPtr( entt::entity id ) noexcept
	{
		if (const auto found = cached_object_pointers.find( id ); found != std::end( cached_object_pointers ))
			return found->second;

		const auto [it, success] = cached_object_pointers.try_emplace( id, new GameObject( *this, id ) );
		ASSERT( success );
		if (!success)
			return {};

		return it->second;
	}

	GameObjectPtr GameObjectStore::CreateObject()
	{
		const auto id = registry.create();
		ASSERT( id != entt::null );

		const auto obj_ptr = GetObjectPtr( id );
		ASSERT( obj_ptr != nullptr );
		ObjectCreated( *obj_ptr );

		return obj_ptr;
	}

	void GameObjectStore::DestroyObject( GameObject& object )
	{
		const auto id = GetInternalId( object );
		ASSERT( id != entt::null ); // should be fine, I just want to debug this
		if (id == entt::null)
			return;

		const auto found_cached_ptr = cached_object_pointers.find( id );
		if (found_cached_ptr != std::end( cached_object_pointers ))
			cached_object_pointers.erase( found_cached_ptr );

		ObjectDestroyed( object );
		registry.destroy( id );
	}
}
