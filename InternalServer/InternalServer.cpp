#include "InternalServer.hpp"

InternalGameServer::InternalGameServer( const uint16_t port )
	: Server::GameServer( yojimbo::Address( "127.0.0.1", port ) )
{
}

InternalGameServer::~InternalGameServer()
{

}
