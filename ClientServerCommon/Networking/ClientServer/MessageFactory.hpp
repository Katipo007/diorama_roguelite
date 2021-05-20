#pragma once

#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/Types.hpp"

namespace Networking::ClientServer
{
	class MessageFactory final
		: public yojimbo::MessageFactory
	{
	public:
		MessageFactory( yojimbo::Allocator& allocator );
		~MessageFactory();

	private:
		yojimbo::Message* CreateMessageInternal( YojimboPlugin::MessageType_T type ) override;
	};
}
