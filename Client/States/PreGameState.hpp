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
		: public StateMachine::DefaultAction<StateMachine::Actions::NoAction>
		, NonCopyable
	{
	public:
		using StateMachine::DefaultAction<StateMachine::Actions::NoAction>::HandleEvent;

		explicit PreGameState();
		~PreGameState();

		StateMachine::Actions::Might<StateMachine::Actions::TransitionTo<MainMenuState>> HandleEvent( const FrameEvent& e );

	protected:
	};
}