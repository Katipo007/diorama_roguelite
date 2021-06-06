#pragma once

#include "ClientServerCommon/ecs.hpp"

#include "ClientServerCommon/Game/ClientSync/ClientSyncTypes.hpp"
#include "ClientServerCommon/Game/ClientSync/SyncableComponents.hpp"
#include "Common/DataTypes/Bytes.hpp"

namespace Game::Networking { struct ConnectionComponent; }

namespace Game::ClientSync::Helpers
{
	void MakeSerialisable( const ecs::EntityHandle entity );
	void Dirty( const ecs::EntityHandle entity, std::optional<ComponentIdentifiers::identifier_type> specific_component = std::nullopt );

	void SyncEntityToClient( ecs::EntityHandle to_sync, Networking::ConnectionComponent& client, const bool reliable = false );
	void RemoveEntityFromClient( ecs::Entity to_remove, Networking::ConnectionComponent& client );

	Buffer_T WriteComponents( const ecs::EntityHandle entity );

	template<SerialisableComponent C>
	inline void WriteComponent( C& component, Serialiser& serialiser, const bool include_identitier = true )
	{
		if (include_identitier)
		{
			static_assert(sizeof( component_id ) == 4);
			serialiser.value4b( ComponentIdentifiers::template type<C> );
		}
		component.Serialise( serialiser );
	}

	template<SerialisableComponent C>
	bool WriteComponent( const ecs::EntityHandle entity, Serialiser& serialiser, const bool include_identifier = true )
	{
		ASSERT( !!entity );
		if (auto* component = entity.try_get<C>())
		{
			WriteComponent<C>( *component, serialiser, include_identifier );
			return true;
		}

		return false;
	}
}
