#include "EntityTemplateStore.hpp"

#include "Common/Utility/Entity/Entity.hpp"

namespace Game
{
	bool EntityTemplateStore::Exists( std::string_view template_id ) const noexcept
	{
		return template_map.contains( template_id );
	}

	entt::entity EntityTemplateStore::GetTemplate( std::string_view template_id )
	{
		const auto found = template_map.find( template_id );
		if (found == std::end( template_map ))
			return entt::null;
		
		return found->second;
	}

	entt::entity EntityTemplateStore::CreateTemplate( std::string_view template_id )
	{
		auto entity = template_registry.create();
		auto emplace_it = template_map.emplace( template_id, entity );
		ASSERT( emplace_it != std::end( template_map ) );

		return entity;
	}

	void EntityTemplateStore::Clear()
	{
		template_map.clear();
		template_registry.clear();
	}

	void EntityTemplateStore::Stomp( entt::handle target, std::string_view template_id )
	{
		if (!target)
			return;

		auto template_entity = GetTemplate( template_id );
		ASSERT( template_registry.valid( template_entity ) );
		if (!template_registry.valid( template_entity ))
			return;

		template_registry.visit( template_entity, [&target](const entt::type_info info ) -> void
			{
				(void)info;
				(void)target;
				NOT_IMPLEMENTED; // Need to work out how entt's meta system works...
			} );
	}
}
