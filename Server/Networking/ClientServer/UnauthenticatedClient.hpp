#pragma once

#include "BaseClientConnection.hpp"

namespace Networking::ClientServer
{
	class UnauthenticatedClient final
		: public BaseClientConnection
	{
	public:
		UnauthenticatedClient( GameServer& owner, YojimboPlugin::ClientIndex_T client_index );
		~UnauthenticatedClient();

		bool HandleMessage( const yojimbo::Message& message ) override;

		std::string_view GetUsername() const;

	private:
		std::string requested_username;
	};
}
