#include "InGameState.hpp"

#include "Visual/Camera.hpp"
#include "Visual/Device/RendererAPI.hpp"
#include "Visual/Device/RendererCommand.hpp"

#include "Client/ClientGame.hpp"
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

	fsm::Actions::NoAction InGameState::OnEnter( const ClientStates::ConnectedToServerEvent& e )
	{
		(void)e;
		return fsm::Actions::NoAction();
	}

	fsm::Actions::TransitionTo<MainMenuState> InGameState::OnLeave( const ClientStates::DisconnectedFromServerEvent& e )
	{
		(void)e;
		return fsm::Actions::TransitionTo<MainMenuState>{};
	}

	fsm::Actions::NoAction ClientStates::InGameState::HandleEvent( const FrameEvent& e )
	{
		(void)e;

		//main_camera->SetRadius( 5.f + std::sin( timestep.time / 4.f ) * 2.f );
		//main_camera->SetRotationD( 45.f, -45.f );
		main_camera->SetRotationD( 95.f, 90.f * std::sin( e.timestep.time / 4 ) );

		return fsm::Actions::NoAction{};
	}

	fsm::Actions::NoAction InGameState::HandleEvent( const RenderEvent& e )
	{
		(void)e;

		OnRender();

		return fsm::Actions::NoAction();
	}

	fsm::Actions::NoAction ClientStates::InGameState::HandleEvent( const DearImGuiFrameEvent& e )
	{
		(void)e;

		return fsm::Actions::NoAction{};
	}

	fsm::Actions::TransitionTo<MainMenuState> InGameState::HandleEvent( const DisconnectedFromServerEvent& e )
	{
		(void)e;
		LOG_INFO( Client, "Lost connection to server, returning to main menu" );
		return fsm::Actions::TransitionTo<MainMenuState>();
	}

	void InGameState::OnRender() const
	{
		gameworld->Render( main_camera );
	}
}
