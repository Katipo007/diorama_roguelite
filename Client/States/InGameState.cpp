#include "InGameState.hpp"

#include "Visual/Camera.hpp"
#include "Visual/Device/RendererAPI.hpp"
#include "Visual/Device/RendererCommand.hpp"

#include "Client/Game/ClientGameWorld.hpp"
#include "Client/Game/PlayerObject.hpp"

namespace ClientStates
{
	InGameState::InGameState()
	{
		main_camera = std::make_shared<Visual::SphericalCamera>();
		main_camera->SetPosition( { 0, 0, 0 } );
		main_camera->SetAspectRatio( 1.f );
		main_camera->SetFOV( 75.f );
		main_camera->SetRadius( 5.f );

		gameworld = std::make_unique<Game::ClientGameWorld>();
	}

	InGameState::~InGameState()
	{
		gameworld.reset();
		main_camera.reset();
	}

	StateMachine::Actions::NoAction ClientStates::InGameState::HandleEvent( const FrameEvent& e )
	{
		(void)e;
		//main_camera->SetRadius( 5.f + std::sin( timestep.time / 4.f ) * 2.f );
		//main_camera->SetRotationD( 45.f, -45.f );
		main_camera->SetRotationD( 95.f, 90.f * std::sin( e.timestep.time / 4 ) );

		return StateMachine::Actions::NoAction{};
	}

	StateMachine::Actions::NoAction InGameState::HandleEvent( const RenderEvent& e )
	{
		(void)e;

		OnRender();

		return StateMachine::Actions::NoAction();
	}

	StateMachine::Actions::NoAction ClientStates::InGameState::HandleEvent( const DearImGuiFrameEvent& e )
	{
		(void)e;

		return StateMachine::Actions::NoAction{};
	}

	//StateMachine::Actions::NoAction ClientStates::InGameState::OnEnter()
	//{
	//	return StateMachine::Actions::NoAction{};
	//}
	//
	//void ClientStates::InGameState::OnLeave()
	//{
	//}

	void InGameState::OnRender() const
	{
		gameworld->Render( main_camera );
	}
}
