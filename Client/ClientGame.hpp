#pragma once

#include <memory>

#include "Common/Utility/NonCopyable.hpp"
#include "Common/Utility/StateMachine/StateMachine.hpp"
#include "Common/Utility/Timestep.hpp"

#include "Client/States/Events.hpp"
#include "Client/States/PreGameState.hpp"
#include "Client/States/InGameState.hpp"

int main( int, char** );

namespace ClientStates
{
	using States = StateMachine::States<
		PreGameState,
		InGameState
	>;

	using Events = StateMachine::Events<
		FrameEvent,
		RenderEvent,
		DearImGuiFrameEvent
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

	protected:
		ClientGame(); // for entry point to call

		void OnFrame( const Timestep& ts ); // for entry point to call

		ClientStates::Machine state_machine;
	};

	ClientGame& GetClientGame();
}
