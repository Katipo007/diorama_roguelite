#pragma once

#include <bitset>
#include <concepts>
#include <functional>

#include "ClientServerCommon/Networking/ClientServer/Config.hpp"
#include "ClientServerCommon/Networking/ClientServer/MessageFactory.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/Types.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/Concepts.hpp"
#include "Common/Utility/Timestep.hpp"
#include "Common/Utility/MagicEnum.hpp"

namespace yojimbo { class Message; }

namespace Networking::ClientServer
{
	class GameServer;

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

		bool SendMessage( ChannelType channel, YojimboPlugin::MessageType_T message_type, const std::function<void(yojimbo::Message&)>& initialiser );
		template<YojimboPlugin::Concepts::Message T>
		bool SendMessage( ChannelType channel, YojimboPlugin::Concepts::MessageInitialiser auto initialiser )
		{
			return SendMessage( channel, MessageFactory::GetMessageType<T>(), [&]( yojimbo::Message& msg ) { initialiser( static_cast<T&>( msg ) ); } );
		}

	protected:
		BaseClientConnection( GameServer& owner, YojimboPlugin::ClientIndex_T client_index );

	protected:
		GameServer& owner;
		const YojimboPlugin::ClientIndex_T client_index;

	private:
		std::bitset<magic_enum::enum_count<Flags>()> flags;
	};
}
