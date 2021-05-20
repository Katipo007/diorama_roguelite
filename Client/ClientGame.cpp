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

#include "Visual/Resources/SpriteSheet.hpp"

#include "ClientServerCommon/Plugins/Yojimbo/Basic/BasicClient.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/YojimboPlugin.hpp"

#include "ClientServerCommon/Networking/ClientServerConfig.hpp"
#include "ClientServerCommon/Networking/MessageFactory.hpp"

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
        , DisconnectedFromServerEvent
    >;

    using Machine = fsm::Machine<States, Events>;


    //namespace
    //{
    //    Game::ClientGame* owner = NULL;
    //    //auto Z = std::tuple<PreGameState, MainMenuState, JoinMultiplayerState, LoadingState, InGameState>();
    //    auto X = std::make_tuple( //std::tuple<PreGameState, MainMenuState, JoinMultiplayerState, LoadingState, InGameState>(
    //        PreGameState()
    //        , MainMenuState( *owner )
    //        , JoinMultiplayerState( *owner )
    //        , LoadingState()
    //        , InGameState( *owner )
    //        );
    //}
}

namespace Game
{
    namespace
    {
        YojimboPlugin::BasicClient::Definition CreateClientDefinition( Plugins::YojimboPlugin& plugin, const YojimboPlugin::Address& target_address )
        {
            auto message_factory = std::make_shared<Game::Networking::GameMessageFactory>();

            return YojimboPlugin::BasicClient::Definition
            {
                .plugin = &plugin,
                .target_address = target_address,
                .private_key = Game::ClientServerConnection::DefaultPrivateKey,
                .config = ClientServerConnection::MakeConfiguration(),
                .adapter = YojimboPlugin::BasicAdapter( message_factory ),
            };
        }
    }


    struct ClientGame::ClientData
    {
        ClientStates::Machine state_machine;

        ClientData( ClientGame& owner )
            : state_machine(
                ClientStates::PreGameState{}
                , ClientStates::MainMenuState{ owner }
                , ClientStates::JoinMultiplayerState{ owner }
                , ClientStates::LoadingState{}
                , ClientStates::InGameState{ owner }
            )
        {
            (void)owner;
        }
    };


    ///
    /// ClientGame
    /// 

    ClientGame::ClientGame()
        : client_data( std::make_unique<ClientData>( *this ) )
    {
    }

    ClientGame::~ClientGame()
    {
        client_data.reset();
    }

    void ClientGame::ConnectionStateChangedHandler( YojimboPlugin::BaseClient& sender )
    {
        ASSERT( client_server_session != nullptr );
        ASSERT( &sender == client_server_session.get() );

        switch (sender.GetState())
        {
        case YojimboPlugin::BaseClient::ConnectionState::Connected:
            client_data->state_machine.Handle( ClientStates::ConnectedToServerEvent( sender ) );
            break;

        case YojimboPlugin::BaseClient::ConnectionState::Connecting:
            // TODO: show some indicator
            break;

        case YojimboPlugin::BaseClient::ConnectionState::Disconnected:
            // NOTHING, will get caught in the new Update() event
            break;
        }
    }

    YojimboPlugin::BaseClient* ClientGame::GetClientServerSession()
    {
        if (client_server_session)
            return client_server_session.get();
        
        return nullptr;
    }

    YojimboPlugin::BaseClient* ClientGame::GetClientServerSession() const
    {
        if (client_server_session)
            return client_server_session.get();

        return nullptr;
    }

    void ClientGame::ConnectToServer( const YojimboPlugin::Address& address )
    {
        if (client_server_session)
        {
            LOG_WARN( Client, "Already have an active server connection" );
            return;
        }

        try
        {
            auto& yojimbo_plugin = core->GetRequiredAPI<Plugins::YojimboPlugin>();

            client_server_session.reset( new YojimboPlugin::BasicClient( CreateClientDefinition( yojimbo_plugin, address ) ) );
            client_server_session->ConnectionStateChanged.connect( &ClientGame::ConnectionStateChangedHandler, this );
        }
        catch (std::exception& e)
        {
            LOG_ERROR( Client, "Failed to initalise the connection to '{}'. What: {}", address, e.what() );
        }
    }

    void ClientGame::DisconnectFromServer()
    {
        ASSERT( client_server_session != nullptr );
        if (client_server_session == nullptr)
            return;

        // disconnect handler before disconnecting
        client_server_session->ConnectionStateChanged.disconnect( this );

        if (client_server_session->GetState() != YojimboPlugin::BaseClient::ConnectionState::Disconnected)
            client_server_session->Disconnect();

        auto e = ClientStates::DisconnectedFromServerEvent( *client_server_session.get() );
        client_data->state_machine.Handle( e );
        auto& yojimbo = core->GetRequiredAPI<Plugins::YojimboPlugin>();
        yojimbo.Remove( *client_server_session );
        client_server_session.reset();
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
    }

    void ClientGame::OnGameEnd()
    {
    }

    void ClientGame::OnFixedUpdate( const PreciseTimestep& ts )
    {
        // Destroy the connection object if it isn't connected
        if (client_server_session && (client_server_session->GetState() == YojimboPlugin::BaseClient::ConnectionState::Disconnected))
            DisconnectFromServer();

        client_data->state_machine.Handle( ClientStates::FrameEvent( ts ) );
    }

    void ClientGame::OnVariableUpdate( const PreciseTimestep& ts )
    {
        (void)ts;
        DoDearImGuiFrame();
    }

    void ClientGame::OnRender( const PreciseTimestep& ts )
    {
        (void)ts; // TODO: use
        client_data->state_machine.Handle( ClientStates::RenderEvent() );
    }

    void ClientGame::DoDearImGuiFrame()
    {
        if (!dearimgui || !dearimgui->GetEnabled())
            return;

        client_data->state_machine.Handle( ClientStates::DearImGuiFrameEvent( *dearimgui ) );
    }
}
