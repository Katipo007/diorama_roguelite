#pragma once

#include "Client/States/Events.hpp"
#include "Common/Utility/StateMachine/DefaultAction.hpp"
#include "Common/Utility/StateMachine/Might.hpp"
#include "Common/Utility/StateMachine/TransitionTo.hpp"
#include "Common/Utility/NonCopyable.hpp"

namespace ClientStates
{
	class JoinMultiplayerState;

	/// <summary>
	/// Loading screen going from menu to in-game
	/// </summary>
	class DisconnectedFromServerState final
		: public fsm::DefaultAction<fsm::NoAction>
	{
	public:
		using fsm::DefaultAction<fsm::NoAction>::HandleEvent;

		explicit DisconnectedFromServerState() = default;
		explicit DisconnectedFromServerState( DisconnectedFromServerState&& ) = default;
		~DisconnectedFromServerState() = default;

		fsm::NoAction OnEnter( const DisconnectedFromServerState& e );
		void OnLeave();

		fsm::Might<fsm::TransitionTo<JoinMultiplayerState>> HandleEvent( const DearImGuiFrameEvent& e );

	private:
		std::string given_reason;
	};
}
