#pragma once

#include "Client/States/Events.hpp"
#include "Common/Utility/StateMachine/DefaultAction.hpp"
#include "Common/Utility/StateMachine/Might.hpp"
#include "Common/Utility/StateMachine/TransitionTo.hpp"

namespace ClientStates
{
	class MainMenuState;

	/// <summary>
	/// Start-up loading screen
	/// </summary>
	class PreGameState final
		: public fsm::DefaultAction<fsm::NoAction>
	{
	public:
		using fsm::DefaultAction<fsm::NoAction>::HandleEvent;

		explicit PreGameState();
		explicit PreGameState( PreGameState&& to_move ) = default;
		~PreGameState();

		fsm::Might<fsm::TransitionTo<MainMenuState>> HandleEvent( const FrameEvent& e );

	protected:

	private:
		PreGameState( const PreGameState& ) = delete;
		PreGameState operator=( const PreGameState& ) = delete;
	};
}
