#pragma once

#include "ClientServerCommon/Game/Components/Concepts.hpp"
#include "Common/Utility/Entity/EnttHeader.hpp"
#include "Common/Utility/StringHashMap.hpp"

namespace Game
{
	// Stores pre-fab entities designed to create entities in other registries from.
	class EntityTemplateStore final
	{
	public:
		bool Exists( std::string_view template_id ) const noexcept;
		entt::entity GetTemplate( std::string_view template_id );

		entt::registry& GetTemplateRegistry() noexcept { return template_registry; }
		const entt::registry& GetTemplateRegistry() const noexcept { return template_registry; }

		entt::entity CreateTemplate( std::string_view template_id );
		void Clear();

		void Stomp( entt::handle target, std::string_view template_id );

		template<Components::Component T, typename... Args>
		void Assign( std::string_view template_id, Args&&... args ) { Assign<T>( GetTemplate( template_id ), std::forward<Args>( args )... ); }
		template<Components::Component T, typename... Args>
		void Assign( entt::entity& template_entity, Args&&... args )
		{
			ASSERT( template_registry.valid( template_entity ) );
			template_registry.emplace<T>( template_entity, std::forward<Args>( args )... );
		}

		template<Components::Component T, typename... Args>
		void AssignOrReplace( std::string_view template_id, Args&&... args ) { AssignOrReplace<T>( GetTemplate( template_id ), std::forward<Args>( args )... ); }
		template<Components::Component T, typename... Args>
		void AssignOrReplace( entt::entity& template_entity, Args&&... args )
		{
			ASSERT( template_registry.valid( template_entity ) );
			template_registry.emplace_or_replace<T>( template_entity, std::forward<Args>( args )... );
		}

		template<Components::Component T>
		bool Has( std::string_view template_id ) const { return Has<T>( GetTemplate( template_id ) ); }
		template<Components::Component T>
		bool Has( entt::entity& template_entity ) const
		{
			ASSERT( template_registry.valid( template_entity ) );
			return (template_registry.try_get<T>( template_entity ) != nullptr);
		}

		template<Components::Component T>
		T& Get( std::string_view template_id ) { return Get<T>( GetTemplate( template_id ) ); }
		template<Components::Component T>
		T& Get( entt::entity& template_entity )
		{
			return template_registry.get<T>( template_entity );
		}

	private:
		StringHashMap<entt::entity> template_map; // maps template names/ids to template entities
		entt::registry template_registry;
	};
}
