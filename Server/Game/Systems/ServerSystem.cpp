#include "ServerSystem.hpp"

#include "ClientServerCommon/Game/Networking/Channels.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"
#include "Server/Game/Components/ServerClientConnection.hpp"

namespace
{
	bool ProcessClientMessage( ecs::EntityHandle& entity, Game::Components::ServerClientConnection& client, const yojimbo::Message& message )
	{
		NOT_IMPLEMENTED;
		(void)entity;
		(void)client;
		(void)message;

		return false;
	}

	bool ProcessMessagesForClient( yojimbo::Server& server, ecs::EntityHandle entity, Game::Components::ServerClientConnection& client )
	{
		for (auto channel : magic_enum::enum_values<Game::Networking::ChannelType>())
		{
			const auto channel_idx = static_cast<YojimboPlugin::ChannelIndex_T>(channel);
			yojimbo::Message* message = server.ReceiveMessage( client.client_index, channel_idx );
			while (message != NULL)
			{
				const bool handled = ProcessClientMessage( entity, client, *message );
				server.ReleaseMessage( client.client_index, message );

				if (!handled)
					return false;
			}
		}

		return true;
	}
}

namespace Game::Systems
{
	void ServerProcessIncoming( yojimbo::Server& server, ecs::Registry& registry, const PreciseTimestep& )
	{
		if (server.IsRunning())
		{
			server.ReceivePackets();

			registry.view<Components::ServerClientConnection>().each( [&]( const auto entity, Components::ServerClientConnection& client )
				{
					if (!ProcessMessagesForClient( server, ecs::EntityHandle{ registry, entity }, client ))
					{
						LOG_WARN( LoggingChannels::Server, "Disconnecting client ({}) due to an unhandled message", client.client_index );
						server.DisconnectClient( client.client_index );
						return; // entity is deleted at this point, no further processing should be done.
					}
				} );
		}
	}

	void ServerProcessOutgoing( yojimbo::Server& server, ecs::Registry&, const PreciseTimestep& ts )
	{
		if( server.IsRunning() )
			server.SendPackets();

		server.AdvanceTime( server.GetTime() + ts.delta );
	}
}
