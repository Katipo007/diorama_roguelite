#include "ClientGame.hpp"

#include "Common/Core/Core.hpp"
#include "Common/Core/ResourceManager.hpp"
#include "Common/Core/API/DearImGuiAPI.hpp"
#include "Common/Core/API/NetworkAPI.hpp"
#include "Common/Core/API/VideoAPI.hpp"
#include "Common/Utility/StateMachine/StateMachine.hpp"
#include "Common/Utility/Timestep.hpp"

#include "Client/States/Events.hpp"
#include "Client/States/PreGameState.hpp"
#include "Client/States/MainMenuState.hpp"
#include "Client/States/JoinMultiplayerState.hpp"
#include "Client/States/LoadingState.hpp"
#include "Client/States/InGameState.hpp"

#include "Visual/Window.hpp"
#include "Visual/Resources/SpriteSheet.hpp"

#include "ClientServerCommon/Networking/ClientServerConfig.hpp"
#include "Common/Networking/Client.hpp"

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

    void ClientGame::ConnectionStateChangedHandler( Networking::Client& sender )
    {
        ASSERT( client_server_session != nullptr );
        ASSERT( &sender == client_server_session.get() );

        switch (sender.GetState())
        {
        case Networking::Client::ConnectionState::Connected:
            client_data->state_machine.Handle( ClientStates::ConnectedToServerEvent( sender ) );
            break;

        case Networking::Client::ConnectionState::Connecting:
            // TODO: show some indicator
            break;

        case Networking::Client::ConnectionState::Disconnected:
            // NOTHING, will get caught in the new Update() event
            break;
        }
    }

    Networking::Client* ClientGame::GetClientServerSession()
    {
        if (client_server_session)
            return client_server_session.get();
        
        return nullptr;
    }

    Networking::Client* ClientGame::GetClientServerSession() const
    {
        if (client_server_session)
            return client_server_session.get();

        return nullptr;
    }

    void ClientGame::ConnectToServer( const ::Networking::Address& address )
    {
        if (client_server_session)
        {
            LOG_WARN( Client, "Already have an active server connection" );
            return;
        }

        try
        {
            auto* network = core->GetAPI<API::NetworkAPI>();
            if (!network)
                throw std::runtime_error( "No networking API initialised" );

            Networking::ClientProperties properties;
            properties.target_address = address;
            properties.private_key = Game::ClientServerConnection::DefaultPrivateKey;

            client_server_session = network->CreateClient( std::move( properties ) );
            client_server_session->ConnectionStateChanged.connect( &ClientGame::ConnectionStateChangedHandler, this );
        }
        catch (std::exception& e)
        {
            LOG_ERROR( Client, "Failed to initalise the connection to '{}'. What: {}", address.value, e.what() );
        }
    }

    void ClientGame::DisconnectFromServer()
    {
        ASSERT( client_server_session != nullptr );
        if (client_server_session == nullptr)
            return;

        // disconnect handler before disconnecting
        client_server_session->ConnectionStateChanged.disconnect( this );

        if (client_server_session->GetState() != Networking::Client::ConnectionState::Disconnected)
            client_server_session->Disconnect();

        auto e = ClientStates::DisconnectedFromServerEvent( *client_server_session.get() );
        client_data->state_machine.Handle( e );
        client_server_session.reset();
    }

    void ClientGame::Init()
    {
        ASSERT( core != nullptr );
        auto* video = core->GetAPI<API::VideoAPI>();
        ASSERT( video != nullptr );
        dearimgui = core->GetAPI<API::DearImGuiAPI>();

        // Preload sprite sheets
        resource_manager->Preload<Graphics::SpriteSheet>( "Art/2DArt/texture.json" );
            
        Graphics::WindowDefinition window_def;
        window_def.title = "Diorama Roguelite";
        window_def.size = { 800, 600 };
        video->SetWindow( std::move( window_def ) );

        ASSERT( video->HasWindow() );
    }

    void ClientGame::OnGameEnd()
    {
    }

    void ClientGame::OnFixedUpdate( const PreciseTimestep& ts )
    {
        // Destroy the connection object if it isn't connected
        if (client_server_session && (client_server_session->GetState() == Networking::Client::ConnectionState::Disconnected))
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
