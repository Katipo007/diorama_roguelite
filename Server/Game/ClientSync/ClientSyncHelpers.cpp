#include "ClientSyncHelpers.hpp"

#include "ClientServerCommon/Game/ClientSync/ClientSyncMessages.hpp"
#include "ClientServerCommon/Game/ClientSync/ClientSyncHelpers.hpp"
#include "ClientServerCommon/Game/ClientSync/SyncableComponents.hpp"
#include "Server/Game/Networking/NetworkingHelpers.hpp"
#include "ClientSyncComponent.hpp"

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

		const Bytes component_data = WriteComponents( to_sync );
		if (component_data.empty())
			return;

		const auto message_initialiser = [&]( Messages::ServerClientEntitySync& msg )
		{
			msg.entity_sync_id = sync_component->sync_id;
		};

		Networking::Helpers::SendBlockMessage<Messages::ServerClientEntitySync>( client, reliable ? Networking::ChannelType::Reliable : Networking::ChannelType::Unreliable, message_initialiser, component_data );
		//<Messages::ServerClientEntitySync>(  );
		NOT_IMPLEMENTED;
	}

	Bytes WriteComponents( const ecs::EntityConstHandle entity )
	{
		static thread_local OComponentDataStream stream;
		stream.str( 0 );
		
		if (entity.valid())
		{
			WriteComponent<Name::NameComponent>( entity, stream );
		}

		const auto data{ stream.str() };

		return Bytes{ std::begin( data ), std::end( data ) };
	}
}
