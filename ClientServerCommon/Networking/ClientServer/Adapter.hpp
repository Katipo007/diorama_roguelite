#pragma once

#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/Types.hpp"
#include "Common/Utility/Signal.hpp"

namespace Networking::ClientServer
{
	class ClientServerAdapter final
		: public yojimbo::Adapter
	{
	public:
		ClientServerAdapter();
		~ClientServerAdapter();

#pragma region Server Events
		Signal::signal<ClientServerAdapter&, YojimboPlugin::ClientIndex_T> ServerClientConnected;
		Signal::signal<ClientServerAdapter&, YojimboPlugin::ClientIndex_T> ServerClientDisconnected;
#pragma endregion

	private:
		yojimbo::MessageFactory* CreateMessageFactory( yojimbo::Allocator& allocator ) override;

		// TODO: override ServerSendLoopbackPacket and ClientSendLoopbackPacket methods for internal server

		void OnServerClientConnected( YojimboPlugin::ClientIndex_T client_index ) override;
		void OnServerClientDisconnected( YojimboPlugin::ClientIndex_T client_index ) override;
	};
}
