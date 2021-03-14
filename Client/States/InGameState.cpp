#include "Precompiled/pch.hpp"
#include "InGameState.hpp"

namespace States
{
	InGameState::InGameState()
		: ClientState( ClientStates::InGame )
	{
		LOG_TRACE( "InGameState created" );
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
	}

	void InGameState::OnDearImGuiRender()
	{
	}
}
