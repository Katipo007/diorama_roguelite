#pragma once

#include <memory>
#include <type_traits>
#include <queue>

#include "Common/Utility/NonCopyable.hpp"
#include "Common/Utility/StateMachine/StateMachine.hpp"
#include "Common/Utility/Timestep.hpp"

#include "Client/States/Events.hpp"
#include "Client/States/PreGameState.hpp"
#include "Client/States/MainMenuState.hpp"
#include "Client/States/JoinMultiplayerState.hpp"
#include "Client/States/LoadingState.hpp"
#include "Client/States/InGameState.hpp"

int main( int, char** );

namespace ClientStates
{
	using States = StateMachine::States<
		PreGameState,
		MainMenuState,
		JoinMultiplayerState,
		LoadingState,
		InGameState
	>;

	using Events = StateMachine::Events<
		FrameEvent,
		RenderEvent,
		DearImGuiFrameEvent,
		ConnectedToServerEvent,
		DisconnectedFromServerEvent
	>;

	using Machine = StateMachine::Machine<States, Events>;
}

namespace Game
{
	class ClientGame final
		: NonCopyable
	{
		friend int ::main( int, char** );

	public:
		~ClientGame();

		bool ShouldExit() const { return user_requested_exit; }
		void Exit();

		void QueueEvent( ClientStates::Machine::EventsVariant_T event );

	protected:
		ClientGame(); // for entry point to call

		void OnFrame( const PreciseTimestep& ts ); // for entry point to call
		void OnDearImGuiFrame();

		std::queue<ClientStates::Machine::EventsVariant_T> state_machine_events_queue;
		ClientStates::Machine state_machine;
		bool user_requested_exit = false;
	};

	ClientGame& GetClientGame();
}
