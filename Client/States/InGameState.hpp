#pragma once

#include <memory>

#include "Client/States/Events.hpp"
#include "Common/Utility/StateMachine/DefaultAction.hpp"
#include "Common/Utility/StateMachine/Actions/Might.hpp"
#include "Common/Utility/StateMachine/Actions/TransitionTo.hpp"
#include "Common/Utility/NonCopyable.hpp"

namespace Visual
{
	class Camera;
	class SphericalCamera;
}

namespace Game
{
	class PlayerObject;
	class ClientGameWorld;
}

namespace ClientStates
{
	class InGameState
		: public StateMachine::DefaultAction<StateMachine::Actions::NoAction>
		, NonCopyable
	{
	public:
		using StateMachine::DefaultAction<StateMachine::Actions::NoAction>::HandleEvent;

		explicit InGameState();
		virtual ~InGameState();

		StateMachine::Actions::NoAction HandleEvent( const FrameEvent& e );
		StateMachine::Actions::NoAction HandleEvent( const RenderEvent& e );
		StateMachine::Actions::NoAction HandleEvent( const DearImGuiFrameEvent& e );

		//StateMachine::Actions::NoAction OnEnter();
		//void OnLeave();

	protected:
		void OnRender() const;

		std::shared_ptr<Visual::SphericalCamera> main_camera;
		std::unique_ptr<Game::ClientGameWorld> gameworld; // TODO: refactor into client session
	};
}
