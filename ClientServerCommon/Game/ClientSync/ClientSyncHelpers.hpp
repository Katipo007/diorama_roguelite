#pragma once

#include "ClientSyncTypes.hpp"
#include "ClientServerCommon/ecs.hpp"
#include "Common/DataTypes/Bytes.hpp"

namespace Game::ClientSync::Helpers
{
	template<SerialisableComponent C>
	inline OComponentDataStream& WriteComponent( const C& component, OComponentDataStream& stream ) { return component.Serialise( stream ); }

	template<SerialisableComponent C>
	OComponentDataStream& WriteComponent( const ecs::EntityConstHandle entity, OComponentDataStream& stream )
	{
		ASSERT( entity.valid() );
		if (auto* component = entity.try_get<C>())
			return WriteComponent<C>( stream, *component );
		return false;
	}

	template<SerialisableComponent C>
	inline IComponentDataStream& ReadComponent( C& component, IComponentDataStream& stream ) { return component.Serialise( stream ); }

	template<SerialisableComponent C>
	IComponentDataStream& ReadComponent( const ecs::EntityConstHandle entity, IComponentDataStream& stream )
	{
		ASSERT( entity.valid() );
		auto& component = entity.get_or_emplace<C>();
		return ReadComponent( stream, component );
	}
}
