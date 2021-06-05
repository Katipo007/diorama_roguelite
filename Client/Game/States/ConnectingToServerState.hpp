#pragma once

#include "Events.hpp"
#include "Common/Utility/StateMachine/DefaultAction.hpp"
#include "Common/Utility/StateMachine/Might.hpp"
#include "Common/Utility/StateMachine/TransitionTo.hpp"
#include "Common/Utility/NonCopyable.hpp"

namespace Game::Networking { class ClientServerSession; }

namespace Game::Events { struct ConnectingToServerEvent; }

namespace Game::States
{
	class DisconnectedFromServerState;
	class JoinMultiplayerState;

	/// <summary>
	/// Loading screen going from menu to in-game
	/// </summary>
	class ConnectingToServerState final
		: public fsm::DefaultAction<fsm::NoAction>
		, NonCopyable
	{
	public:
		using fsm::DefaultAction<fsm::NoAction>::HandleEvent;

		fsm::NoAction OnEnter( const Events::ConnectingToServerEvent& );
		void OnLeave();

		fsm::NoAction HandleEvent( const Events::DearImGuiFrameEvent& e );
		fsm::TransitionTo<DisconnectedFromServerState> HandleEvent( const Events::DisconnectedFromServerEvent& e );

	private:
		Networking::ClientServerSession* client_server_session = nullptr;
	};
}
