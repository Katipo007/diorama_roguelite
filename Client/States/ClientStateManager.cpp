#include "Precompiled/pch.hpp"
#include "ClientStateManager.hpp"
#include "ClientState.hpp"

#include "Visual/DearImGui/DearImGui.hpp"

namespace States
{
	ClientStateManager::ClientStateManager()
	{
		// TODO: create states
	}

	ClientStateManager::~ClientStateManager()
	{
		states.clear();
	}

	ClientState* ClientStateManager::Get( const ClientStates type )
	{
		const auto it = std::find_if( std::begin( states ), std::end( states ), [ type ]( const decltype(states)::value_type& state )
			{
				return state && state->GetType() == type;
			} );

		if (it != std::end( states ))
			return it->get();

		return nullptr;
	}

	const ClientState* ClientStateManager::Get( const ClientStates type ) const
	{
		const auto it = std::find_if( std::begin( states ), std::end( states ), [type]( const decltype(states)::value_type& state )
			{
				return state && state->GetType() == type;
			} );

		if (it != std::end( states ))
			return it->get();

		return nullptr;
	}

	void ClientStateManager::ForEach( const std::function<void( ClientState& )>& func )
	{
		for (auto& state : states)
		{
			ASSERT( state != nullptr );
			func( *state );
		}
	}

	void ClientStateManager::ForEach( const std::function<void( const ClientState& )>& func ) const
	{
		for (auto& state : states)
		{
			ASSERT( state != nullptr );
			func( *state );
		}
	}

	void ClientStateManager::BringToTop( ClientState& state )
	{
		const auto it = std::find_if( std::begin(states), std::end(states), [&state]( const decltype(states)::value_type& entry ) { return &state == entry.get(); } );
		if (it == std::end( states ))
			throw std::runtime_error( "Given state does not belong to this manager!" );

		const size_t index = std::distance( std::begin( states ), it );
		std::rotate( std::begin( states ) + index, std::begin( states ) + index + 1, std::end( states ) );
	}

	void ClientStateManager::SendToBack( ClientState& state )
	{
		const auto it = std::find_if( std::begin( states ), std::end( states ), [&state]( const decltype(states)::value_type& entry ) { return &state == entry.get(); } );
		if (it == std::end( states ))
			throw std::runtime_error( "Given state does not belong to this manager!" );

		const size_t index = std::distance( std::begin( states ), it );
		std::rotate( std::rend( states ) - index - 1, std::rend( states ) - index, std::rend( states ) );
	}

	void ClientStateManager::OnFrame( const Timestep& ts )
	{
		for (auto& state : states)
		{
			if (state->IsEnabled())
				state->OnFrame( ts );
		}

#ifdef DEARIMGUI_ENABLED
		if (DearImGui::IsEnabled())
		{
			for (auto& state : states)
			{
				if (state->IsEnabled())
					state->OnDearImGuiRender();
			}
		}
#endif
	}
}
