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

	void ClientServerAdapter::OnServerClientConnected( YojimboPlugin::ClientIndex_T client_index )
	{
		ServerClientConnected( *this, client_index );
	}

	void ClientServerAdapter::OnServerClientDisconnected( YojimboPlugin::ClientIndex_T client_index )
	{
		ServerClientDisconnected( *this, client_index );
	}
}
