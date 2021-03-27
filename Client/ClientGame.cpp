#include "ClientGame.hpp"

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

    void ClientGame::OnFrame( const Timestep& ts )
    {
        state_machine.Handle( ClientStates::FrameEvent( ts ) );
        state_machine.Handle( ClientStates::RenderEvent() );
    }

    void ClientGame::OnDearImGuiFrame()
    {
#ifdef DEARIMGUI_ENABLED
        state_machine.Handle( ClientStates::DearImGuiFrameEvent() );
#endif
    }
}
