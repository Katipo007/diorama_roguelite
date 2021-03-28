#pragma once

#include "Server/GameServer.hpp"

class InternalGameServer
	: public Server::GameServer
{
public:
	explicit InternalGameServer( const uint16_t port );
	virtual ~InternalGameServer() override;

	// TODO

protected:
};
