#pragma once

#include "Messages.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/TemplateMessageFactory.hpp"

namespace Game::Networking
{
	using MessageFactory = YojimboPlugin::TemplateMessageFactory<
		// message types go here
		Messages::DummyMessage
		, Messages::ServerClientDisconnect

		, Messages::ClientServerLoginStart
		, Messages::ServerClientLoginSuccess

		, Messages::ClientServerChatMessage
		, Messages::ServerClientChatMessage
		, ClientSync::Messages::ServerClientEntitySync
		>;
}
