#include "NetworkingHelpers.hpp"

#include "ClientServerCommon/Game/Components/Name.hpp"
#include "Server/Game/Components/ActiveClient.hpp"
#include "Server/Game/Components/PendingClient.hpp"
#include "Server/Game/Components/ServerPlayer.hpp"
#include "Server/Game/Components/ServerClientConnection.hpp"
#include "Common/Utility/StringUtility.hpp"

namespace Game::Helpers
{
	void AcceptClient( ecs::EntityHandle entity, std::string_view username )
	{
		ASSERT( entity.valid() );
		if (!entity.valid())
			return;

		auto& connection = entity.get<Components::ServerClientConnection>();

		entity.remove<Components::PendingClient>();
		entity.emplace<Components::ActiveClient>();
		entity.emplace<Components::ServerPlayer>();
		const auto& name = entity.emplace<Components::Name>( Components::Name{ .value = username.data() } );
		
		SendMessage<Networking::Messages::ServerClientLoginSuccess>( connection, Networking::ChannelType::Reliable, [&]( Networking::Messages::ServerClientLoginSuccess& message )
			{
				StringUtility::StringToArray( name.value, message.username );
			} );

		LOG_INFO( LoggingChannels::Server, "Accepted login from ({})", connection.client_index );
	}

	void DisconnectClient( ecs::EntityHandle entity, std::optional<std::string_view> reason )
	{
		ASSERT( entity.valid() );
		if (!entity.valid())
			return;

		auto& connection = entity.get<Components::ServerClientConnection>();

		SendMessage<Networking::Messages::ServerClientDisconnect>( connection, Networking::ChannelType::Reliable, [&]( Networking::Messages::ServerClientDisconnect& message )
			{
				if (reason)
					StringUtility::StringToArray( *reason, message.reason );
			} );

		LOG_INFO( LoggingChannels::Server, "Disconnecting client ({}), reason: '{}'", connection.client_index, reason ? *reason : "<None>" );

		connection.GetOwner().DisconnectClient( connection.client_index );
	}

	void BroadcastChatMessage( ecs::EntityHandle from, std::string_view message )
	{
		ASSERT( from.valid() );

		if (auto* name = from.try_get<Components::Name>())
			BroadcastChatMessage( *from.registry(), name->value, message );
	}

	void BroadcastChatMessage( ecs::Registry& registry, std::string_view sender, std::string_view message )
	{
		if (sender.empty() || message.empty())
			return;

		registry.view<Components::ServerClientConnection, Components::ActiveClient>().each( [&]( auto, Components::ServerClientConnection& connection, Components::ActiveClient& )
			{
				SendMessage<Networking::Messages::ServerClientChatMessage>( connection, Networking::ChannelType::Unreliable, [&]( Networking::Messages::ServerClientChatMessage& chat )
					{
						StringUtility::StringToArray( sender, chat.sender );
						StringUtility::StringToArray( message, chat.message );
					} );
			} );
	}
}
