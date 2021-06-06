#include "ClientSyncHelpers.hpp"

#include "ClientServerCommon/Game/ClientSync/ClientSyncMessages.hpp"
#include "ClientServerCommon/Game/ClientSync/SyncableComponents.hpp"
#include "Server/Game/Networking/NetworkingHelpers.hpp"
#include "ClientSyncComponent.hpp"

namespace
{
	Game::ClientSync::EntitySyncId GetNextSyncId()
	{
		static Game::ClientSync::EntitySyncId id = 0;
		return ++id;
	}
}

namespace Game::ClientSync::Helpers
{
	void SyncEntityToClient( ecs::EntityHandle to_sync, Networking::ConnectionComponent& client, const bool reliable )
	{
		if (!to_sync)
			return;

		auto* sync_component = to_sync.try_get<ClientSyncComponent>();
		ASSERT( sync_component );
		if (!sync_component)
			return;

		ASSERT( sync_component->sync_id != 0 );
		if (sync_component->sync_id == 0)
			return;

		const auto component_data = WriteComponents( to_sync );
		if (component_data.empty())
			return;

		const auto message_initialiser = [&]( Messages::ServerClientEntitySync& msg )
		{
			msg.entity_sync_id = sync_component->sync_id;
		};

		Networking::Helpers::SendBlockMessage<Messages::ServerClientEntitySync>( client, reliable ? Networking::ChannelType::Reliable : Networking::ChannelType::Unreliable, message_initialiser, (uint8_t*)component_data.data(), std::size( component_data ) );
	}

	void MakeSerialisable( const ecs::EntityHandle entity )
	{
		auto& sync = entity.get_or_emplace<ClientSyncComponent>();
		if (sync.sync_id == 0)
			sync.sync_id = GetNextSyncId();

		Dirty( entity );
	}

	void Dirty( const ecs::EntityHandle entity, std::optional<ComponentIdentifiers::identifier_type> specific_component )
	{
		if (auto* sync = entity.try_get<ClientSyncComponent>())
		{
			if (specific_component)
				NOT_IMPLEMENTED;
			else if( !sync->dirty )
				sync->dirty = true;
		}
	}

	Buffer_T WriteComponents( const ecs::EntityHandle entity )
	{
		Buffer_T data;
		WriterS serialiser{ data };
		
		if (entity.valid())
		{
			WriteComponent<Name::NameComponent>( entity, serialiser );
		}

		serialiser.adapter().flush();
		const auto used_num_bytes = serialiser.adapter().writtenBytesCount();
		data.resize( used_num_bytes );
		return data;
	}
}
