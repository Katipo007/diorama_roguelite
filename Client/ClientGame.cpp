#include "ClientGame.hpp"

#include "Common/Core/Core.hpp"
#include "Common/Core/ResourceManager.hpp"
#include "Common/Core/API/DearImGuiAPI.hpp"
#include "Common/Core/API/VideoAPI.hpp"
#include "Common/Core/Graphics/Window.hpp"
#include "Common/Utility/StateMachine/StateMachine.hpp"
#include "Common/Utility/Timestep.hpp"

#include "Client/Game/States/Events.hpp"
#include "Client/Game/States/PreGameState.hpp"
#include "Client/Game/States/MainMenuState.hpp"
#include "Client/Game/States/JoinMultiplayerState.hpp"
#include "Client/Game/States/ConnectingToServerState.hpp"
#include "Client/Game/States/DisconnectedFromServerState.hpp"
#include "Client/Game/States/InGameState.hpp"
#include "Client/Game/States/ExitGameState.hpp"

#include "Client/Game/Networking/ClientIdGenerator.hpp"
#include "Client/Game/Networking/ClientServerSession.hpp"

#include "Visual/Resources/SpriteSheet.hpp"

namespace
{
    using States = fsm::States<
        Game::States::PreGameState
        , Game::States::MainMenuState
        , Game::States::JoinMultiplayerState
        , Game::States::ConnectingToServerState
        , Game::States::DisconnectedFromServerState
        , Game::States::InGameState
        , Game::States::ExitGameState
    >;

    using Events = fsm::Events<
        Game::Events::FrameEvent
        , Game::Events::RenderEvent
        , Game::Events::DearImGuiFrameEvent
        , Game::Events::ConnectedToServerEvent
        , Game::Events::ConnectingToServerEvent
        , Game::Events::DisconnectedFromServerEvent
    >;

    using Client = fsm::Machine<States, Events>;
}

struct ClientGame::Pimpl
{
    Client client{};
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

void ClientGame::ConnectToServer( Game::Networking::ServerConnectionRequest&& request )
{
    if (client_server_session != nullptr)
        DisconnectFromServer();

    ASSERT( client_server_session == nullptr );
    client_server_session.reset( new Game::Networking::ClientServerSession{ std::move( request ) } );
    client_server_session->ConnectionStateChanged.connect( &ClientGame::ServerConnectionStateChangedHandler, this );
    
    pimpl->client.Handle( Game::Events::ConnectingToServerEvent{ *client_server_session } );
}

void ClientGame::DisconnectFromServer()
{
    if (client_server_session == nullptr)
        return;

    pimpl->client.Handle( Game::Events::DisconnectedFromServerEvent{ *client_server_session } );
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

    pimpl.reset( new Pimpl() );

    // temporary solution
    pimpl->client.GetState<Game::States::JoinMultiplayerState>().ConnectToServerClicked.connect( [this]( Game::Networking::ServerConnectionRequest request )
        {
            ConnectToServer( std::move( request ) );
        } );
}

void ClientGame::OnGameEnd()
{
    pimpl.reset();
}

void ClientGame::OnFixedUpdate( const PreciseTimestep& ts )
{
    ASSERT( pimpl );
    if (client_server_session)
        client_server_session->OnFixedUpdate( ts );

    pimpl->client.Handle( Game::Events::FrameEvent{ ts } );

    if (client_server_session && client_server_session->IsDisconnected())
        DisconnectFromServer();

    if (pimpl->client.IsInState<Game::States::ExitGameState>())
        Exit( 0 );
}

void ClientGame::OnVariableUpdate( const PreciseTimestep& )
{
    ASSERT( pimpl );
    DoDearImGuiFrame();
}

void ClientGame::OnRender( const PreciseTimestep& )
{
    ASSERT( pimpl );
    pimpl->client.Handle( Game::Events::RenderEvent{} );
}

void ClientGame::DoDearImGuiFrame()
{
    ASSERT( pimpl );
    if (!dearimgui || !dearimgui->GetEnabled())
        return;

    pimpl->client.Handle( Game::Events::DearImGuiFrameEvent{ *dearimgui } );
}

void ClientGame::ServerConnectionStateChangedHandler( Game::Networking::ClientServerSession& session )
{
    if (client_server_session && (&session == client_server_session.get()))
    {
        if (session.IsConnected())
            pimpl->client.Handle( Game::Events::ConnectedToServerEvent{ session } );
    }
}
