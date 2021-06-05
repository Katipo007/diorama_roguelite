#pragma once

#include "Events.hpp"
#include "Client/Game/Networking/ServerConnectionRequest.hpp"

#include "Common/Utility/StateMachine/DefaultAction.hpp"
#include "Common/Utility/StateMachine/Might.hpp"
#include "Common/Utility/StateMachine/TransitionTo.hpp"

#include "Common/Utility/NonCopyable.hpp"
#include "Common/Utility/Signal.hpp"

namespace Game::States
{
	class MainMenuState;
	class ConnectingToServerState;

	/// <summary>
	/// Screen to join an existing multi-player game
	/// </summary>
	class JoinMultiplayerState final
		: public fsm::DefaultAction<fsm::NoAction>
		, NonCopyable
	{
		using ExitActions = fsm::OneOf< fsm::NoAction,
			fsm::TransitionTo<MainMenuState>,
			fsm::TransitionTo<ConnectingToServerState>
		>;

	public:
		using fsm::DefaultAction<fsm::NoAction>::HandleEvent;

		ExitActions HandleEvent( const Events::FrameEvent& e );
		ExitActions HandleEvent( const Events::DearImGuiFrameEvent& e );
		ExitActions HandleEvent( const Events::ConnectedToServerEvent& e );
		ExitActions HandleEvent( const Events::ConnectingToServerEvent& e );


		Signal::signal<Networking::ServerConnectionRequest> ConnectToServerClicked;

	protected:
		void ConnectToServer();

	protected:
		std::string status_message;
	};
}
