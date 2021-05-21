#pragma once

#include "Messages.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/TemplateMessageFactory.hpp"

namespace Networking::ClientServer
{
	using MessageFactory = YojimboPlugin::TemplateMessageFactory<
		// message types go here
		Dummy
		>;
}
