#include "ClientConnectionYojimbo.hpp"

#include "ServerYojimbo.hpp"
#include "YojimboHeader.hpp"

namespace Plugins::Network::Yojimbo
{
	ClientConnectionYojimbo::ClientConnectionYojimbo( ServerYojimbo& owner_, ::Networking::ClientId_T id_, int yojimbo_client_index )
		: ClientConnection( dynamic_cast<::Networking::Server&>( owner_ ) )
		, owner( owner_ )
	{
	}

	ClientConnectionYojimbo::~ClientConnectionYojimbo()
	{
	}
}
