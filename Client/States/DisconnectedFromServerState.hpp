#pragma once

#include "Client/States/Events.hpp"
#include "Common/Utility/StateMachine/DefaultAction.hpp"
#include "Common/Utility/StateMachine/Actions/Might.hpp"
#include "Common/Utility/StateMachine/Actions/TransitionTo.hpp"
#include "Common/Utility/NonCopyable.hpp"

namespace ClientStates
{
	class JoinMultiplayerState;

	/// <summary>
	/// Loading screen going from menu to in-game
	/// </summary>
	class DisconnectedFromServerState final
		: public fsm::DefaultAction<fsm::Actions::NoAction>
	{
	public:
		using fsm::DefaultAction<fsm::Actions::NoAction>::HandleEvent;

		explicit DisconnectedFromServerState() = default;
		explicit DisconnectedFromServerState( DisconnectedFromServerState&& ) = default;
		~DisconnectedFromServerState() = default;

		fsm::Actions::NoAction OnEnter( const DisconnectedFromServerState& e );
		void OnLeave();

		fsm::Actions::Might<fsm::Actions::TransitionTo<JoinMultiplayerState>> HandleEvent( const DearImGuiFrameEvent& e );

	private:
		std::string given_reason;
	};
}
