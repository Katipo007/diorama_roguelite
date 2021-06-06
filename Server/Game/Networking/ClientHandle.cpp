#include "ClientHandle.hpp"

#include "Server/Game/Server.hpp"

namespace Game::Networking
{
    ClientHandle::ClientHandle()
    {
    }

    ClientHandle::ClientHandle( Server& server, ClientId client_id )
        : server( &server )
        , client_id( client_id )
    {
    }

    bool ClientHandle::IsNull() const noexcept
    {
        return (server == nullptr)
            || (client_id == 0)
            || (!server->GetServer().IsRunning())
            || (!server->GetServer().IsClientConnected( GetClientIndex() ))
            ;
    }

    ClientIndex ClientHandle::GetClientIndex() const noexcept
    {
        NOT_IMPLEMENTED;
        return ClientIndex();
    }
}
