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
	using CStates = StateMachine::States<
		PreGameState,
		InGameState
	>;

	using CEvents = StateMachine::Events<
		FrameEvent,
		RenderEvent,
		DearImGuiFrameEvent
	>;

	using CStateMachine = StateMachine::Machine<CStates, CEvents>;
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

		ClientStates::CStateMachine client_state;
	};

	ClientGame& GetClientGame();
}
