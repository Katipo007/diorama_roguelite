#pragma once

#include "ClientServerCommon/ecs.hpp"

#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"
#include "ClientServerCommon/Game/Networking/MessageFactory.hpp"
#include "ClientServerCommon/Game/Networking/Channels.hpp"
#include "Server/Game/Components/ServerClientConnection.hpp"
#include "Common/Utility/MagicEnum.hpp"

namespace Game::Helpers
{
	void AcceptClient( ecs::EntityHandle entity, std::string_view username );
	void DisconnectClient( ecs::EntityHandle entity, std::optional<std::string_view> reason = std::nullopt );

	void BroadcastChatMessage( ecs::EntityHandle from, std::string_view message );
	void BroadcastChatMessage( ecs::Registry& registry, std::string_view sender, std::string_view message );

	template<YojimboPlugin::Concepts::Message T>
	bool SendMessage( Components::ServerClientConnection& connection, Networking::ChannelType channel, std::invocable<T&> auto initialiser )
	{
		auto& server = connection.GetOwner();
		const auto channel_idx = magic_enum::enum_integer( channel );

		if (!server.IsClientConnected( connection.client_index ) || !server.CanSendMessage( connection.client_index, channel_idx ))
			return false;

		if (auto* message = static_cast<T*>(server.CreateMessage( connection.client_index, Networking::MessageFactory::GetMessageType<T>() )))
		{
			initialiser( *message );
			server.SendMessage( connection.client_index, channel_idx, message );
			return true;
		}
		
		FATAL( "Failed to allocate message" );
	}
}
