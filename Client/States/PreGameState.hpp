#pragma once

#include "Client/States/Events.hpp"
#include "Common/Utility/StateMachine/DefaultAction.hpp"
#include "Common/Utility/StateMachine/Actions/Might.hpp"
#include "Common/Utility/StateMachine/Actions/TransitionTo.hpp"
#include "Common/Utility/NonCopyable.hpp"

namespace ClientStates
{
	class InGameState;

	class PreGameState
		: public StateMachine::DefaultAction<StateMachine::Actions::NoAction>
		, NonCopyable
	{
	public:
		using StateMachine::DefaultAction<StateMachine::Actions::NoAction>::HandleEvent;

		explicit PreGameState();
		~PreGameState();

		StateMachine::Actions::Might<StateMachine::Actions::TransitionTo<InGameState>> HandleEvent( const FrameEvent& e );

	protected:
	};
}
