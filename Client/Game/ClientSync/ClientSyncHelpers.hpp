#pragma once

#include "ClientServerCommon/Game/ClientSync/ClientSyncTypes.hpp"
#include "ClientServerCommon/Game/ClientSync/SyncableComponents.hpp"
#include "ClientServerCommon/ecs.hpp"
#include "Common/DataTypes/Bytes.hpp"

namespace Game::ClientSync::Helpers
{
	void UpdateEntity( const ecs::EntityHandle entity, Buffer_T& component_data );

	template<SerialisableComponent C>
	inline void ReadComponent( C& component, ReaderS& serialiser, const bool expects_identifier = true )
	{
		if (expects_identifier)
		{
			constexpr auto expected = ComponentIdentifiers::template type<C>;
			ComponentIdentifiers::identifier_type actual;
			static_assert(sizeof( actual ) == 4);
			serialiser.value4b( actual );
			ASSERT( actual == expected );
			if (actual != expected)
				throw std::runtime_error{ "Mismatched component type!" };
		}
		return component.Serialise( serialiser );
	}

	template<SerialisableComponent C>
	void ReadComponent( const ecs::EntityHandle entity, ReaderS& serialiser, const bool expects_identifier = true )
	{
		ASSERT( entity.valid() );
		auto& component = entity.get_or_emplace<C>();
		return ReadComponent( component, serialiser, expects_identifier );
	}
}
