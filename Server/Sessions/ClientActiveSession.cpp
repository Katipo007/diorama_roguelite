#include "ClientActiveSession.hpp"

#include "Server/Networking/ClientServer/ServerClientConnection.hpp"

namespace Sessions
{
    ClientActiveSession::ClientActiveSession( std::unique_ptr<Networking::ClientServer::ServerClientConnection> connection_ )
        : BaseServerClientSession( std::move( connection_ ) )
    {
    }

    ClientActiveSession::~ClientActiveSession()
    {
    }

    bool ClientActiveSession::HandleMessage( const yojimbo::Message& )
    {
        return false;
    }
}
