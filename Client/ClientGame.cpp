#include "ClientGame.hpp"

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

    void ClientGame::QueueEvent( ClientStates::Machine::EventsVariant_T event )
    {
        state_machine_events_queue.push( std::move( event ) );
    }

    void ClientGame::OnFrame( const PreciseTimestep& ts )
    {
        // TODO: Don't clear here, require individual states to clear the screen
        Visual::Device::RendererCommand::SetClearColour( ColourRGBA( 0, 0, 0, 255 ) );
        Visual::Device::RendererCommand::Clear();

        state_machine.Handle( ClientStates::FrameEvent( ts ) );
        state_machine.Handle( ClientStates::RenderEvent() );

        // process events queue
        while (!state_machine_events_queue.empty())
        {
            auto top = std::move( state_machine_events_queue.front() );
            state_machine_events_queue.pop();

            std::visit( [ this ]( auto&& e ) -> void { state_machine.Handle( e ); }, top );
        }
    }

    void ClientGame::OnDearImGuiFrame()
    {
#ifdef DEARIMGUI_ENABLED
        state_machine.Handle( ClientStates::DearImGuiFrameEvent() );
#endif
    }
}
