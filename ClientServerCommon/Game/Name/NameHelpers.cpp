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

	std::string_view GetName( const ecs::EntityConstHandle entity )
	{
		static const std::string_view empty = "";
		static const std::string_view invalid = "<INVALID>";

		if (!entity)
			return invalid;

		if (auto* name_component = entity.try_get<NameComponent>())
			return name_component->value;

		return empty;
	}
}
