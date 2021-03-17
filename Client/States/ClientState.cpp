#include "ClientState.hpp"

namespace States
{
	ClientState::ClientState( const ClientStates type_ )
		: type( type_ )
	{

	}

	ClientState::~ClientState()
	{
	}

	bool ClientState::IsEnabled() const
	{
		return is_enabled;
	}

	void ClientState::Enable()
	{
		ASSERT( !is_enabled );
		if (is_enabled)
			return;

		is_enabled = true;
		this->OnEnabled();
	}

	void ClientState::Disable()
	{
		ASSERT( is_enabled );
		if (!is_enabled)
			return;

		is_enabled = false;
		this->OnDisabled();
	}
}
