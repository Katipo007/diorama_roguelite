#include "GameObject.hpp"

namespace Game
{
	GameObject::GameObject( GameObjectStore& owner, entt::entity id )
		: Owner( owner )
		, id( id )
	{
	}

	GameObject::~GameObject() = default;

	bool GameObject::IsNull() const noexcept
	{
		return (id != NullGameObjectId) && Owner.Exists( *this );
	}
}
