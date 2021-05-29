#pragma once

#include <bitset>

#include "ClientServerCommon/Plugins/Yojimbo/Types.hpp"
#include "Common/Utility/Timestep.hpp"
#include "Common/Utility/MagicEnum.hpp"

class GameServer;

namespace yojimbo { class Message; }

namespace Networking::ClientServer
{
	class BaseClientConnection
	{
	public:
		enum class Flags
		{
			ToBeDisconnected,
			LoopbackClient,
		};

	public:
		virtual ~BaseClientConnection();

		virtual bool HandleMessage( const yojimbo::Message& message ) = 0;
		virtual void OnFixedUpdate( const PreciseTimestep& ) {}

		YojimboPlugin::ClientIndex_T GetClientIndex() const noexcept { return client_index; }
		GameServer& GetOwner() noexcept { return owner; }
		const GameServer& GetOwner() const noexcept { return owner; }

		bool TestFlag( Flags flag ) const noexcept;
		void SetFlag( Flags flag, const bool value = true ) noexcept;
		void ClearFlag( Flags flag ) noexcept { SetFlag( flag, false ); }

	protected:
		BaseClientConnection( GameServer& owner, YojimboPlugin::ClientIndex_T client_index );

	protected:
		GameServer& owner;
		const YojimboPlugin::ClientIndex_T client_index;

	private:
		std::bitset<magic_enum::enum_count<Flags>()> flags;
	};
}
