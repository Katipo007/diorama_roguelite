#pragma once

#include "Events.hpp"
#include "Common/Utility/StateMachine/DefaultAction.hpp"
#include "Common/Utility/StateMachine/Might.hpp"
#include "Common/Utility/StateMachine/TransitionTo.hpp"
#include "Common/Utility/NonCopyable.hpp"

namespace Game::States
{
	class JoinMultiplayerState;

	/// <summary>
	/// Loading screen going from menu to in-game
	/// </summary>
	class DisconnectedFromServerState final
		: public fsm::DefaultAction<fsm::NoAction>
		, NonCopyable
	{
	public:
		using fsm::DefaultAction<fsm::NoAction>::HandleEvent;

		fsm::NoAction OnEnter( const Events::DisconnectedFromServerEvent& e );
		void OnLeave();

		fsm::Might<fsm::TransitionTo<JoinMultiplayerState>> HandleEvent( const Events::DearImGuiFrameEvent& e );

	private:
		std::optional<std::string> given_reason;
	};
}
