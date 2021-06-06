#pragma once

#include "ClientServerCommon/Game/Networking.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"
#include "Common/Utility/Signal.hpp"

namespace Game::Networking
{
	class Adapter final
		: public yojimbo::Adapter
	{
	public:
		Adapter();
		~Adapter();

#pragma region Server Events
		Signal::signal<Adapter&, ClientIndex> ServerClientConnected;
		Signal::signal<Adapter&, ClientIndex> ServerClientDisconnected;
#pragma endregion

	private:
		yojimbo::MessageFactory* CreateMessageFactory( yojimbo::Allocator& allocator ) override;

		// TODO: override ServerSendLoopbackPacket and ClientSendLoopbackPacket methods for internal server

		void OnServerClientConnected( YojimboPlugin::ClientIndex_T client_index ) override;
		void OnServerClientDisconnected( YojimboPlugin::ClientIndex_T client_index ) override;
	};
}
