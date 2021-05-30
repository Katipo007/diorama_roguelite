#pragma once

#include "Client/States/Events.hpp"
#include "Common/Utility/StateMachine/DefaultAction.hpp"
#include "Common/Utility/StateMachine/Actions/Might.hpp"
#include "Common/Utility/StateMachine/Actions/TransitionTo.hpp"
#include "Common/Utility/NonCopyable.hpp"

namespace Game { class ClientGame; }
namespace Networking::ClientServer { class ServerConnection; }

namespace ClientStates
{
	class DisconnectedFromServerState;
	class JoinMultiplayerState;
	class InGameState;

	/// <summary>
	/// Loading screen going from menu to in-game
	/// </summary>
	class ConnectingToServerState final
		: public fsm::DefaultAction<fsm::Actions::NoAction>
	{
	public:
		using fsm::DefaultAction<fsm::Actions::NoAction>::HandleEvent;

		explicit ConnectingToServerState( Game::ClientGame& );
		explicit ConnectingToServerState( ConnectingToServerState&& ) = default;
		~ConnectingToServerState() = default;

		fsm::Actions::NoAction OnEnter( const ConnectingToServerEvent& );
		void OnLeave();

		fsm::Actions::Might<fsm::Actions::TransitionTo<JoinMultiplayerState>> HandleEvent( const DearImGuiFrameEvent& e );
		fsm::Actions::Might<fsm::Actions::TransitionTo<InGameState>> HandleEvent( const ServerMessageEvent& e );
		fsm::Actions::NoAction HandleEvent( const ConnectedToServerEvent& e );
		fsm::Actions::TransitionTo<DisconnectedFromServerState> HandleEvent( const DisconnectedFromServerEvent& e );

	private:
		ConnectingToServerState( const ConnectingToServerState& ) = delete;
		ConnectingToServerState operator=( const ConnectingToServerState& ) = delete;

		void AttachToConnection( Networking::ClientServer::ServerConnection& connection );
		void DetatchFromConnection( Networking::ClientServer::ServerConnection& connection );

	private:
		Game::ClientGame& game;
		Networking::ClientServer::ServerConnection* connection = nullptr;
	};
}
