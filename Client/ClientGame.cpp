#include "ClientGame.hpp"

#include "Client/States/InGameState.hpp"

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

    ClientGame::~ClientGame()
    {
        static_client_game_ptr = nullptr;
    }

    ClientGame::ClientGame()
    {
        ASSERT( static_client_game_ptr == nullptr ); // only one instance should ever exist
        static_client_game_ptr = this;
    }

    void ClientGame::OnFrame( const Timestep& ts )
    {
        client_state.Handle( ClientStates::FrameEvent( ts ) );
        client_state.Handle( ClientStates::RenderEvent() );
    }
}
