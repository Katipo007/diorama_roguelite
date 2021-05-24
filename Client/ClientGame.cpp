#include "ClientGame.hpp"

#include "Common/Core/Core.hpp"
#include "Common/Core/ResourceManager.hpp"
#include "Common/Core/API/DearImGuiAPI.hpp"
#include "Common/Core/API/VideoAPI.hpp"
#include "Common/Core/Graphics/Window.hpp"
#include "Common/Utility/StateMachine/StateMachine.hpp"
#include "Common/Utility/Timestep.hpp"

#include "Client/States/Events.hpp"
#include "Client/States/PreGameState.hpp"
#include "Client/States/MainMenuState.hpp"
#include "Client/States/JoinMultiplayerState.hpp"
#include "Client/States/LoadingState.hpp"
#include "Client/States/InGameState.hpp"

#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"
#include "Client/Networking/ClientServer/ServerConnection.hpp"

#include "Visual/Resources/SpriteSheet.hpp"

namespace ClientStates
{
    using States = fsm::States<
        PreGameState
        , MainMenuState
        , JoinMultiplayerState
        , LoadingState
        , InGameState
    >;

    using Events = fsm::Events<
        FrameEvent
        , RenderEvent
        , DearImGuiFrameEvent
        , ConnectedToServerEvent
        , ConnectingToServerEvent
        , DisconnectedFromServerEvent
        , ServerMessageEvent
    >;

    using Machine = fsm::Machine<States, Events>;
}

namespace Game
{
    struct ClientGame::Pimpl
    {
        ClientStates::Machine state_machine;
        Networking::ClientServer::ServerConnection server_connection;

        Pimpl( ClientGame& owner )
            : state_machine(
                ClientStates::PreGameState{}
                , ClientStates::MainMenuState{ owner }
                , ClientStates::JoinMultiplayerState{ owner }
                , ClientStates::LoadingState{}
                , ClientStates::InGameState{ owner }
            )
            , server_connection( std::bind( &ClientGame::ServerConnectionMessageHandler, &owner, std::placeholders::_1, std::placeholders::_2 ) )
        {
            (void)owner;
        }
    };


    ///
    /// ClientGame
    /// 

    ClientGame::ClientGame()
    {
    }

    ClientGame::~ClientGame()
    {
    }

    Networking::ClientServer::ServerConnection& ClientGame::GetServerConnection()
    {
        return pimpl->server_connection;
    }

    const Networking::ClientServer::ServerConnection& ClientGame::GetServerConnection() const
    {
        return pimpl->server_connection;
    }

    bool ClientGame::IsConnectedToServer() const noexcept
    {
        return pimpl->server_connection.IsConnected();
    }

    void ClientGame::ConnectToServer( std::string_view address )
    {
        yojimbo::Address target_address( address.data() );

        DisconnectFromServer();
        if (target_address.IsValid())
            pimpl->server_connection.Connect( target_address );
        else
            LOG_WARN( Client, "Failed to connect to server, invalid address" );
    }

    void ClientGame::DisconnectFromServer( std::optional<std::string> reason )
    {
        if( !pimpl->server_connection.IsDisconnected() )
            pimpl->server_connection.Disconnect();
    }

    void ClientGame::Init()
    {
        ASSERT( core != nullptr );
        auto& video = core->GetRequiredAPI<API::VideoAPI>();
        dearimgui = core->GetAPI<API::DearImGuiAPI>();

        // Preload sprite sheets
        resource_manager->Preload<Graphics::SpriteSheet>( "Art/2DArt/texture.json" );
            
        Graphics::WindowDefinition window_def;
        window_def.title = "Diorama Roguelite";
        window_def.size = { 800, 600 };
        video.SetWindow( std::move( window_def ) );

        ASSERT( video.HasWindow() );

        pimpl.reset( new Pimpl( *this ) );
        pimpl->server_connection.ConnectionStateChanged.connect( &ClientGame::ServerConnectionStateChangedHandler, this );
    }

    void ClientGame::OnGameEnd()
    {
        pimpl.reset();
    }

    void ClientGame::OnFixedUpdate( const PreciseTimestep& ts )
    {
        if (pimpl->state_machine.IsInState<ClientStates::MainMenuState>() && !pimpl->server_connection.IsDisconnected())
            pimpl->server_connection.Disconnect();

        pimpl->server_connection.OnFixedUpdate( ts );
        pimpl->state_machine.Handle( ClientStates::FrameEvent( ts ) );
    }

    void ClientGame::OnVariableUpdate( const PreciseTimestep& ts )
    {
        (void)ts;
        DoDearImGuiFrame();
    }

    void ClientGame::OnRender( const PreciseTimestep& ts )
    {
        (void)ts; // TODO: use
        pimpl->state_machine.Handle( ClientStates::RenderEvent() );
    }

    void ClientGame::DoDearImGuiFrame()
    {
        if (!dearimgui || !dearimgui->GetEnabled())
            return;

        pimpl->state_machine.Handle( ClientStates::DearImGuiFrameEvent( *dearimgui ) );
    }

    void ClientGame::ServerConnectionStateChangedHandler( Networking::ClientServer::ServerConnection& connection )
    {
        if (&connection == &pimpl->server_connection)
        {
            switch (connection.GetClientState())
            {
            case yojimbo::ClientState::CLIENT_STATE_CONNECTED:
                pimpl->state_machine.Handle( ClientStates::ConnectedToServerEvent{ connection } );
                break;

            case yojimbo::ClientState::CLIENT_STATE_CONNECTING:
                pimpl->state_machine.Handle( ClientStates::ConnectingToServerEvent{ connection } );
                break;

            case yojimbo::ClientState::CLIENT_STATE_DISCONNECTED:
                pimpl->state_machine.Handle( ClientStates::DisconnectedFromServerEvent{ connection } );
                break;

            case yojimbo::ClientState::CLIENT_STATE_ERROR:
                NOT_IMPLEMENTED;
                break;
            }
        }
    }

    bool ClientGame::ServerConnectionMessageHandler( Networking::ClientServer::ServerConnection& connection, const yojimbo::Message& message )
    {
        if (&connection == &pimpl->server_connection)
        {
            ClientStates::ServerMessageEvent e( connection, message );

            pimpl->state_machine.Handle( e );

            return e.handled;
        }

        return false;
    }
}
