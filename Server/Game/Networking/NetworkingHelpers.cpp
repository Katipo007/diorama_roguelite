#include "NetworkingHelpers.hpp"

#include "ClientServerCommon/Game/Name/NameComponent.hpp"
#include "Server/Game/ClientSync/ClientSyncHelpers.hpp"
#include "ActiveClientComponent.hpp"
#include "PendingClientComponent.hpp"
#include "ConnectionComponent.hpp"
#include "Common/Utility/StringUtility.hpp"

namespace Game::Networking::Helpers
{
	void AcceptClient( ecs::EntityHandle entity, std::string_view username )
	{
		ASSERT( entity.valid() );
		if (!entity.valid())
			return;

		auto& connection = entity.get<ConnectionComponent>();

		entity.remove<PendingClientComponent>();
		entity.emplace<ActiveClientComponent>();
		auto& name = entity.emplace<Name::NameComponent>();
		StringUtility::StringToArray( username, name.value );
		ClientSync::Helpers::MakeSerialisable( entity );
		
		SendMessage<Networking::Messages::ServerClientLoginSuccess>( connection, Networking::ChannelType::Reliable, [&]( Networking::Messages::ServerClientLoginSuccess& message )
			{
				StringUtility::StringToArray( name.value.data(), message.username );
			} );

		LOG_INFO( LoggingChannels::Server, "Accepted login from ({})", connection.client_index );
	}

	void DisconnectClient( ecs::EntityHandle entity, std::optional<std::string_view> reason )
	{
		ASSERT( entity.valid() );
		if (!entity.valid())
			return;

		auto& connection = entity.get<ConnectionComponent>();

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

		if (auto* name = from.try_get<Name::NameComponent>())
			BroadcastChatMessage( *from.registry(), name->value.data(), message );
	}

	void BroadcastChatMessage( ecs::Registry& registry, std::string_view sender, std::string_view message )
	{
		if (sender.empty() || message.empty())
			return;

		registry.view<ConnectionComponent, ActiveClientComponent>().each( [&]( auto, ConnectionComponent& connection, ActiveClientComponent& )
			{
				SendMessage<Networking::Messages::ServerClientChatMessage>( connection, Networking::ChannelType::Unreliable, [&]( Networking::Messages::ServerClientChatMessage& chat )
					{
						StringUtility::StringToArray( sender, chat.sender );
						StringUtility::StringToArray( message, chat.message );
					} );
			} );
	}
}
