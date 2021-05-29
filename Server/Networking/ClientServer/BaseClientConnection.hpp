#pragma once

#include "ClientServerCommon/Plugins/Yojimbo/Types.hpp"
#include "Common/Utility/Timestep.hpp"

class GameServer;

namespace yojimbo { class Message; }

namespace Networking::ClientServer
{
	class BaseClientConnection
	{
	public:
		BaseClientConnection( GameServer& owner, YojimboPlugin::ClientIndex_T client_index );
		virtual ~BaseClientConnection();

		virtual bool HandleMessage( const yojimbo::Message& message ) = 0;
		virtual void OnFixedUpdate( const PreciseTimestep& ) {}

		YojimboPlugin::ClientIndex_T GetClientIndex() const noexcept { return client_index; }
		GameServer& GetOwner() noexcept { return owner; }
		const GameServer& GetOwner() const noexcept { return owner; }

	protected:
		GameServer& owner;
		const YojimboPlugin::ClientIndex_T client_index;
	};
}
