#include "ClientConnectionYojimbo.hpp"

#include "ServerYojimbo.hpp"
#include "YojimboHeader.hpp"

namespace Plugins::Network::Yojimbo
{
	ClientConnectionYojimbo::ClientConnectionYojimbo( ServerYojimbo& owner_, ::Networking::ClientId_T id_, int yojimbo_client_index_ )
		: ClientConnection( dynamic_cast<::Networking::Server&>( owner_ ) )
		, owner( owner_ )
	{
		// TODO: Use or remove
		(void)id_;
		(void)yojimbo_client_index_;
	}

	ClientConnectionYojimbo::~ClientConnectionYojimbo()
	{
	}
}
