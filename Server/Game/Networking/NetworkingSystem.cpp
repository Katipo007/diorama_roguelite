#include "NetworkingSystem.hpp"

#include "ClientServerCommon/Game/Networking/Channels.hpp"
#include "ClientServerCommon/Game/Networking/MessageFactory.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"
#include "Components/ConnectionComponent.hpp"
#include "Components/PendingClientComponent.hpp"
#include "Components/ActiveClientComponent.hpp"
#include "NetworkingHelpers.hpp"

#include <chrono>

namespace
{
	using namespace Game::Networking;

	template<typename T>
	constexpr auto MsgType = Game::Networking::MessageFactory::GetMessageType<T>();

	bool ProcessPendingClientMessage( ecs::EntityHandle& entity, ConnectionComponent&, const yojimbo::Message& message )
	{
		using namespace Game::Networking::Messages;
		switch (message.GetType())
		{
		case MsgType<ClientServerLoginStart>:
		{
			const auto& request = static_cast<const ClientServerLoginStart&>(message);

			const std::string_view requested_name{ request.username.data() };

			if (requested_name.length() >= 3)
				Helpers::AcceptClient( entity, requested_name );
			else
				Helpers::DisconnectClient( entity, "Invalid requested username, name too short" );
			return true;
		}
		}

		return false;
	}

	bool ProcessActiveClientMessage( ecs::EntityHandle& entity, ConnectionComponent& client, const yojimbo::Message& message )
	{
		(void)client;

		using namespace Game::Networking::Messages;
		switch (message.GetType())
		{
		case MsgType<ClientServerChatMessage>:
		{
			const auto& chat = static_cast<const ClientServerChatMessage&>(message);
			Helpers::BroadcastChatMessage( entity, chat.message.data() );
			return true;
		}
		}

		return false;
	}

	bool ProcessMessagesForClient( yojimbo::Server& server, ecs::EntityHandle entity, ConnectionComponent& client )
	{
		for (auto channel : magic_enum::enum_values<Game::Networking::ChannelType>())
		{
			const auto channel_idx = static_cast<YojimboPlugin::ChannelIndex_T>(channel);
			yojimbo::Message* message = server.ReceiveMessage( client.client_index, channel_idx );
			while (message != NULL)
			{
				const auto message_type = message->GetType();
				bool handled = false;

				if (auto* pending = entity.try_get<PendingClientComponent>())
				{
					handled |= ProcessPendingClientMessage( entity, client, *message );
				}
				else if (auto* active = entity.try_get<ActiveClientComponent>())
				{
					handled |= ProcessActiveClientMessage( entity, client, *message );
				}

				server.ReleaseMessage( client.client_index, message );

				if (!handled)
				{
					LOG_ERROR( LoggingChannels::Server, "Unhandled message of type '{}'({})", MessageFactory::GetMessageName( message_type ), message_type );
					return false;
				}

				message = server.ReceiveMessage( client.client_index, channel_idx );
			}
		}

		return true;
	}
}

namespace Game::Networking
{
	void IncomingSystem( yojimbo::Server& server, ecs::Registry& registry, const PreciseTimestep& )
	{
		if (server.IsRunning())
		{
			server.ReceivePackets();

			const auto now = std::chrono::system_clock::now();
			registry.view<ConnectionComponent>().each( [&]( const auto entity, ConnectionComponent& client )
				{
					if (auto* pending = registry.try_get<PendingClientComponent>( entity ))
					{
						if (std::chrono::duration_cast<std::chrono::seconds>(now - client.connected_at).count() > 5)
						{
							LOG_INFO( LoggingChannels::Server, "Disconnecting client ({}) due to taking too long to log in", client.client_index );
							server.DisconnectClient( client.client_index );
							return; // entity is deleted at this point, no further processing should be done.
						}
					}

					if (!ProcessMessagesForClient( server, ecs::EntityHandle{ registry, entity }, client ))
					{
						LOG_WARN( LoggingChannels::Server, "Disconnecting client ({}) due to an unhandled message", client.client_index );
						server.DisconnectClient( client.client_index );
						return; // entity is deleted at this point, no further processing should be done.
					}
				} );
		}
	}

	void OutgoingSystem( yojimbo::Server& server, ecs::Registry&, const PreciseTimestep& ts )
	{
		if( server.IsRunning() )
			server.SendPackets();

		server.AdvanceTime( server.GetTime() + ts.delta );
	}
}
