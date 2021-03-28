#include "ClientGame.hpp"

#include "Client/Sessions/ClientServerSession.hpp"

#include "Visual/Device/RendererCommand.hpp"

namespace
{
    static Game::ClientGame* static_client_game_ptr = nullptr;
}

namespace Game
{
    ClientGame& Game::GetClientGame()
    {
        ASSERT( static_client_game_ptr != nullptr );
        return *static_client_game_ptr;
    }


    ///
    /// ClientGame
    /// 

    ClientGame::ClientGame()
    {
        ASSERT( static_client_game_ptr == nullptr ); // only one instance should ever exist
        static_client_game_ptr = this;
    }

    ClientGame::~ClientGame()
    {
        static_client_game_ptr = nullptr;
    }

    void ClientGame::Exit()
    {
        user_requested_exit = true;
    }

    void ClientGame::OnFrame( const PreciseTimestep& ts )
    {
        if (client_server_session)
        {
            if (client_server_session->IsDisconnected())
                DisconnectFromServer();
            else
                client_server_session->Update( ts );
        }

        // TODO: Don't clear here, require individual states to clear the screen
        Visual::Device::RendererCommand::SetClearColour( ColourRGBA( 0, 0, 0, 255 ) );
        Visual::Device::RendererCommand::Clear();

        state_machine.Handle( ClientStates::FrameEvent( ts ) );
        state_machine.Handle( ClientStates::RenderEvent() ); // TODO: separate rendering from steps
    }

    void ClientGame::OnDearImGuiFrame()
    {
#ifdef DEARIMGUI_ENABLED
        state_machine.Handle( ClientStates::DearImGuiFrameEvent() );
#endif
    }

    void ClientGame::ConnectionStateChangedHandler( Sessions::ClientServerSession& sender )
    {
        ASSERT( client_server_session != nullptr );
        ASSERT( &sender == client_server_session.get() );

        switch (sender.GetConnectionState())
        {
        case Sessions::ClientServerSession::ConnectionState::Connected:
            state_machine.Handle( ClientStates::ConnectedToServerEvent( &sender ) );
            break;

        case Sessions::ClientServerSession::ConnectionState::Connecting:
            break;

        case Sessions::ClientServerSession::ConnectionState::Disconnected:
            // NOTHING, will get caught in the new Update() event
            break;
        }
    }

    void ClientGame::ConnectToServer( const yojimbo::Address& address )
    {
        if (client_server_session)
        {
            LOG_WARN( Client, "Already have an active server connection" );
            return;
        }

        try
        {
            client_server_session = std::make_unique<Sessions::ClientServerSession>( address );
            client_server_session->ConnectionStateChanged.connect( &ClientGame::ConnectionStateChangedHandler, this );
        }
        catch (std::exception& e)
        {
            char address_str[128];
            address.ToString( address_str, sizeof( address_str ) );

            LOG_ERROR( Client, "Failed to initalise the connection to '{}'. What: {}", address_str, e.what() );
        }
    }

    void ClientGame::DisconnectFromServer()
    {
        ASSERT( client_server_session != nullptr );
        if (client_server_session == nullptr)
            return;

        // disconnect handler before disconnecting
        client_server_session->ConnectionStateChanged.disconnect( this );

        if (!client_server_session->IsDisconnected())
            client_server_session->Disconnect();


        auto e = ClientStates::DisconnectedFromServerEvent( client_server_session.get() );
        state_machine.Handle( e );
        client_server_session.reset();
    }
}
