#include "ActiveClient.hpp"

#include "UnauthenticatedClient.hpp"

namespace Networking::ClientServer
{
	ActiveClient::ActiveClient( UnauthenticatedClient& unauthed_client )
		: BaseClientConnection( unauthed_client.GetOwner(), unauthed_client.GetClientIndex() )
	{
	}
	
	ActiveClient::~ActiveClient() = default;

	bool ActiveClient::HandleMessage( const yojimbo::Message& )
	{
		return false;
	}
}
