#pragma once

#include "ClientServerCommon/Game/Networking.hpp"

namespace Game { class Server; }

namespace Game::Networking
{
	class ClientHandle final
	{
	public:
		static const ClientHandle Null;

	public:
		ClientHandle();
		ClientHandle( Server& server, ClientId client_id );

		bool IsNull() const noexcept;

		ClientIndex GetClientIndex() const noexcept;

	private:
		Server* server = nullptr;
		ClientId client_id;
	};
}
