#include "Adapter.hpp"

#include "MessageFactory.hpp"

namespace Game::Networking
{
	Adapter::Adapter()
		: yojimbo::Adapter()
	{
	}

	Adapter::~Adapter()
	{
	}

	yojimbo::MessageFactory* Adapter::CreateMessageFactory( yojimbo::Allocator& allocator )
	{
		return YOJIMBO_NEW( allocator, MessageFactory, allocator );
	}

	void Adapter::OnServerClientConnected( YojimboPlugin::ClientIndex_T client_index )
	{
		ServerClientConnected( *this, static_cast<ClientIndex>( client_index ) );
	}

	void Adapter::OnServerClientDisconnected( YojimboPlugin::ClientIndex_T client_index )
	{
		ServerClientDisconnected( *this, static_cast<ClientIndex>(client_index) );
	}
}
