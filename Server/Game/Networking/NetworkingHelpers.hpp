#pragma once

#include "ClientServerCommon/ecs.hpp"

#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"
#include "ClientServerCommon/Game/Networking/MessageFactory.hpp"
#include "ClientServerCommon/Game/Networking/Channels.hpp"
#include "Components/ConnectionComponent.hpp"
#include "Common/DataTypes/Bytes.hpp"
#include "Common/Utility/MagicEnum.hpp"

#include <algorithm>

namespace Game::Networking::Helpers
{
	void AcceptClient( ecs::EntityHandle entity, std::string_view username );
	void DisconnectClient( ecs::EntityHandle entity, std::optional<std::string_view> reason = std::nullopt );

	void BroadcastChatMessage( ecs::EntityHandle from, std::string_view message );
	void BroadcastChatMessage( ecs::Registry& registry, std::string_view sender, std::string_view message );

	template<YojimboPlugin::Concepts::Message T>
	bool SendMessage( ConnectionComponent& connection, Networking::ChannelType channel, std::invocable<T&> auto initialiser )
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

	template<YojimboPlugin::Concepts::BlockMessage T>
	bool SendBlockMessage( ConnectionComponent& connection, Networking::ChannelType channel, std::invocable<T&> auto initialiser, const uint8_t* data, size_t data_size )
	{
		ASSERT( data_size < std::numeric_limits<int>::max() );
		if( data_size >= std::numeric_limits<int>::max())
			throw std::runtime_error{ "Data block too large for message" };

		auto& server = connection.GetOwner();
		const auto client_idx = connection.client_index;
		const auto channel_idx = magic_enum::enum_integer( channel );

		if (!server.IsClientConnected( client_idx ) || !server.CanSendMessage( client_idx, channel_idx ))
			return false;

		if (auto* message = static_cast<T*>(server.CreateMessage( client_idx, Networking::MessageFactory::GetMessageType<T>() )))
		{
			uint8_t* block = server.AllocateBlock( client_idx, static_cast<int>(data_size) );
			if (block == NULL)
				throw std::runtime_error{ "Could not allocate block memory for message" };

			std::copy( data, data + data_size, block );
			initialiser( *message );
			server.AttachBlockToMessage( client_idx, message, block, static_cast<int>(data_size) );

			server.SendMessage( client_idx, channel_idx, message );
			return true;
		}

		FATAL( "Failed to allocate message" );
	}
}
