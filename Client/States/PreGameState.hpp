#pragma once

#include "Client/States/Events.hpp"
#include "Common/Utility/StateMachine/DefaultAction.hpp"
#include "Common/Utility/StateMachine/Actions/Might.hpp"
#include "Common/Utility/StateMachine/Actions/TransitionTo.hpp"
#include "Common/Utility/NonCopyable.hpp"

namespace ClientStates
{
	class MainMenuState;

	/// <summary>
	/// Start-up loading screen
	/// </summary>
	class PreGameState final
		: public fsm::DefaultAction<fsm::Actions::NoAction>
		, NonCopyable
	{
	public:
		using fsm::DefaultAction<fsm::Actions::NoAction>::HandleEvent;

		explicit PreGameState();
		~PreGameState();

		fsm::Actions::Might<fsm::Actions::TransitionTo<MainMenuState>> HandleEvent( const FrameEvent& e );

	protected:
	};
}
