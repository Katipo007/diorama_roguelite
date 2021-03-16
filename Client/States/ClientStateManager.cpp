#include "Precompiled/pch.hpp"
#include "ClientStateManager.hpp"
#include "ClientState.hpp"

#include "Visual/DearImGui/DearImGui.hpp"

namespace States
{
	ClientStateManager::ClientStateManager()
	{
		
	}

	ClientStateManager::~ClientStateManager()
	{
		for (auto type = 0; type < (size_t)(ClientStates::NumClientStates); type++)
		{
			if (auto* state = Get( (ClientStates)type ))
				RemoveState( *state );
		}
		ASSERT( states.empty() );
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
		{
			CLOG_ERROR( "Given state does not belong to this manager!" );
			return;
		}

		const size_t index = std::distance( std::begin( states ), it );
		std::rotate( std::begin( states ) + index, std::begin( states ) + index + 1, std::end( states ) );
	}

	void ClientStateManager::SendToBack( ClientState& state )
	{
		const auto it = std::find_if( std::begin( states ), std::end( states ), [&state]( const decltype(states)::value_type& entry ) { return &state == entry.get(); } );
		if (it == std::end( states ))
		{
			CLOG_ERROR( "Given state does not belong to this manager!" );
			return;
		}

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

	void ClientStateManager::AddState( std::unique_ptr<ClientState> state, const bool start_enabled )
	{
		ASSERT( state != nullptr );
		if (state == nullptr)
			return;

		const auto existing_state = Get( state->GetType() );
		if (existing_state != nullptr)
		{
			CLOG_ERROR( "A state of this type already exists!" );
			return;
		}

		states.push_back( std::move( state ) );
		auto& added_state = states.back();
		added_state->OnAttached();

		if (start_enabled)
			added_state->Enable();
	}

	void ClientStateManager::RemoveState( ClientState& state )
	{
		const auto erase_it = std::remove_if( std::begin( states ), std::end( states ), [&state]( const decltype(states)::value_type& entry ) { return &state == entry.get(); } );
		// disable and detatch about to be deleted states
		for (auto it = erase_it; it != std::end( states ); it++)
		{
			auto& to_remove = *it;
			if (to_remove->IsEnabled())
				to_remove->Disable();

			to_remove->OnDetached();
		}

		states.erase( erase_it, std::end( states ) );
	}

	void ClientStateManager::EnableState( ClientState& state )
	{
		const auto it = std::find_if( std::begin( states ), std::end( states ), [&state]( const decltype(states)::value_type& entry ) { return &state == entry.get(); } );
		if (it == std::end( states ))
		{
			CLOG_ERROR( "Given state does not belong to this manager!" );
			return;
		}

		if( !(*it)->IsEnabled() )
			(*it)->Enable();
	}

	void ClientStateManager::DisableState( ClientState& state )
	{
		const auto it = std::find_if( std::begin( states ), std::end( states ), [&state]( const decltype(states)::value_type& entry ) { return &state == entry.get(); } );
		if (it == std::end( states ))
		{
			CLOG_ERROR( "Given state does not belong to this manager!" );
			return;
		}

		if ((*it)->IsEnabled())
			(*it)->Disable();
	}
}
