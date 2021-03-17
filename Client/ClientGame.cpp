#include "ClientGame.hpp"

#include "Client/States/ClientStateManager.hpp"
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
        state_manager.reset();

        static_client_game_ptr = nullptr;
    }

    ClientGame::ClientGame()
    {
        ASSERT( static_client_game_ptr == nullptr ); // only one instance should ever exist
        static_client_game_ptr = this;

        state_manager = std::make_unique<States::ClientStateManager>();

        state_manager->AddState( std::make_unique<States::InGameState>(), true );
    }

    void ClientGame::OnFrame( const Timestep& ts )
    {
        state_manager->OnFrame( ts );
    }
}
