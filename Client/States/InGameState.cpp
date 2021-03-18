#include "InGameState.hpp"

#include "Visual/Camera.hpp"
#include "Visual/Device/RendererAPI.hpp"
#include "Visual/Device/RendererCommand.hpp"

#include "Client/Game/ClientGameWorld.hpp"
#include "Client/Game/PlayerObject.hpp"

namespace States
{
	InGameState::InGameState()
		: ClientState( ClientStates::InGame )
	{
	}

	InGameState::~InGameState()
	{
	}

	void InGameState::OnAttached()
	{
	}

	void InGameState::OnDetached()
	{
	}

	void InGameState::OnEnabled()
	{
		main_camera = std::make_shared<Visual::SphericalCamera>();
		main_camera->SetPosition( { 0, 0, 0 } );
		main_camera->SetAspectRatio( 1.f );
		main_camera->SetFOV( 75.f );
		main_camera->SetRadius( 5.f );

		gameworld = std::make_unique<Game::ClientGameWorld>();
	}

	void InGameState::OnDisabled()
	{
		main_camera.reset();
		gameworld.reset();
	}

	void InGameState::OnFrame( const Timestep& timestep )
	{
		(void)timestep;
		//main_camera->SetRadius( 5.f + std::sin( timestep.time / 4.f ) * 2.f );
		//main_camera->SetRotationD( 45.f, -45.f );
		main_camera->SetRotationD( 95.f, 90.f * std::sin( timestep.time / 4 ) );

		// TODO: separate to be called independantly of the update loop
		Render();
	}

	void InGameState::OnDearImGuiRender()
	{
	}

	void InGameState::Render() const
	{
		gameworld->Render( main_camera );
	}
}
