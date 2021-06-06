#pragma once

#include "Events.hpp"
#include "Common/Utility/StateMachine/DefaultAction.hpp"
#include "Common/Utility/StateMachine/Might.hpp"
#include "Common/Utility/StateMachine/TransitionTo.hpp"
#include "Common/Utility/NonCopyable.hpp"

namespace Game::States
{
	class MainMenuState;

	/// <summary>
	/// Start-up loading screen
	/// </summary>
	class PreGameState final
		: public fsm::DefaultAction<fsm::NoAction>
		, NonCopyable
	{
	public:
		using fsm::DefaultAction<fsm::NoAction>::HandleEvent;

		fsm::Might<fsm::TransitionTo<MainMenuState>> HandleEvent( const Events::FrameEvent& e );
	};
}
