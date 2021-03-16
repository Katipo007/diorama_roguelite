#include "Precompiled/pch.hpp"
#include "InGameState.hpp"

#include "Client/Game/PlayerObject.hpp"

namespace States
{
	InGameState::InGameState()
		: ClientState( ClientStates::InGame )
	{
		player = std::make_unique<Game::PlayerObject>();

		LOG_TRACE( Client, "Test!" );
		LOG_INFO( Client, "Test!" );
		LOG_WARN( Client, "Test!" );
		LOG_ERROR( Client, "Test!" );
		ASSERT( false );
		ASSERT_CHANNEL( Client, false, "Test!" );
		LOG_CRITICAL( Client, "Test!" );
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
	}

	void InGameState::OnDisabled()
	{
	}

	void InGameState::OnFrame( const Timestep& timestep )
	{
		(void)timestep;
		if (player)
			player->Render();
	}

	void InGameState::OnDearImGuiRender()
	{
	}
}
