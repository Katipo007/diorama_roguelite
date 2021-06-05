#include "ClientSystem.hpp"

#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"
#include "ClientServerCommon/Game/Networking/MessageFactory.hpp"
#include "ClientServerCommon/Game/Networking/Channels.hpp"
#include "Common/Utility/MagicEnum.hpp"

namespace
{
	bool ProcessMessages( yojimbo::Client& client, const std::function<bool( const yojimbo::Message& )>& func )
	{
		for (auto channel : magic_enum::enum_values<Game::Networking::ChannelType>())
		{
			const auto channel_idx = static_cast<YojimboPlugin::ChannelIndex_T>(channel);
			yojimbo::Message* message = client.ReceiveMessage( channel_idx );
			while (message != NULL)
			{
				const auto message_type = message->GetType();
				const bool handled = func( *message );
				client.ReleaseMessage( message );

				if (!handled)
				{
					LOG_WARN( LoggingChannels::Client, "Disconnecting from server due to unhandled message of type '{}'({})", Game::Networking::MessageFactory::GetMessageName(message_type), message_type );
					client.Disconnect();
					return false;
				}
			}
		}

		return true;
	}
}


namespace Game::Systems
{
	void ClientProcessIncoming( yojimbo::Client& client, ecs::Registry&, const PreciseTimestep& )
	{
		if (client.IsConnected())
		{
			client.ReceivePackets();

			ProcessMessages( client, [&]( const yojimbo::Message& ) -> bool
				{


					return false;
				} );
		}
	}

	void ClientProcessOutgoing( yojimbo::Client& client, ecs::Registry&, const PreciseTimestep& ts )
	{
		if (client.IsConnected())
			client.SendPackets();

		client.AdvanceTime( client.GetTime() + ts.delta );
	}
}
