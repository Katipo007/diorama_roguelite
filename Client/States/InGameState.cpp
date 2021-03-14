#include "Precompiled/pch.hpp"
#include "InGameState.hpp"

#include "Client/Game/PlayerObject.hpp"

namespace States
{
	InGameState::InGameState()
		: ClientState( ClientStates::InGame )
	{
		LOG_TRACE( "InGameState created" );

		player = std::make_unique<Game::PlayerObject>();
	}

	InGameState::~InGameState()
	{
		LOG_TRACE( "InGameState destroyed" );
	}

	void InGameState::OnAttached()
	{
		LOG_TRACE( "InGameState attached" );
	}

	void InGameState::OnDetached()
	{
		LOG_TRACE( "InGameState detatched" );
	}

	void InGameState::OnEnabled()
	{
		LOG_TRACE( "InGameState enabled" );
	}

	void InGameState::OnDisabled()
	{
		LOG_TRACE( "InGameState disabled" );
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
