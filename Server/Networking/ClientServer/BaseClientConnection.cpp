#include "BaseClientConnection.hpp"

#include "GameServer.hpp"

namespace Networking::ClientServer
{
	BaseClientConnection::BaseClientConnection( GameServer& owner, YojimboPlugin::ClientIndex_T client_index )
		: owner( owner )
		, client_index( client_index )
	{

	}

	BaseClientConnection::~BaseClientConnection() = default;

	bool BaseClientConnection::TestFlag( Flags flag ) const noexcept
	{
		return flags.test( magic_enum::enum_index<Flags>( flag ).value() );
	}

	void BaseClientConnection::SetFlag( Flags flag, const bool value ) noexcept
	{
		flags.set( magic_enum::enum_index<Flags>( flag ).value(), value );
	}

	bool BaseClientConnection::SendMessage( ChannelType channel, YojimboPlugin::MessageType_T message_type, const std::function<void( yojimbo::Message& )>& initialiser )
	{
		return owner.SendMessage( *this, channel, message_type, initialiser );
	}
}
