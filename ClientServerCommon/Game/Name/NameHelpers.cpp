#include "NameHelpers.hpp"

#include "NameComponent.hpp"

namespace Game::Name::Helpers
{
	void SetName( const ecs::EntityHandle entity, std::string_view name )
	{
		ASSERT( !!entity );
		auto& component = entity.get_or_emplace<NameComponent>();
		component.value = name.substr( 0, NameComponent::MaxNameLength );
	}
}
