#pragma once

#include "Messages.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/Basic/TemplateMessageFactory.hpp"

namespace Game::Networking
{
	using GameMessageFactory = YojimboPlugin::TemplateMessageFactory<
		Dummy
	>;
}
