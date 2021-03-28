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

namespace Sessions
{
	class ClientServerSession;
}

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

		//
		// Client server session management
		//
		Sessions::ClientServerSession* GetClientServerSession() { return client_server_session.get(); }
		const Sessions::ClientServerSession* GetClientServerSession() const { return client_server_session.get(); }
		void ConnectToServer( const yojimbo::Address& address );
		void DisconnectFromServer();

	protected:
		ClientGame(); // for entry point to call

		void OnFrame( const PreciseTimestep& ts ); // for entry point to call
		void OnDearImGuiFrame();

	protected:
		std::unique_ptr<Sessions::ClientServerSession> client_server_session;

		ClientStates::Machine state_machine;
		bool user_requested_exit = false;
	};

	ClientGame& GetClientGame();
}
