#pragma once

#include "Common/Networking/ClientConnection.hpp"

namespace Plugins::Network::Yojimbo
{
	class ServerYojimbo;

	class ClientConnectionYojimbo final
		: public ::Networking::ClientConnection
	{
		friend class ServerYojimbo;
		using ClientContainer_T = std::unordered_map<::Networking::ClientId_T, ClientConnectionYojimbo>;

	public:
		ClientConnectionYojimbo(
			ServerYojimbo& owner
			, ::Networking::ClientId_T id
			, int yojimbo_client_index
		);
		~ClientConnectionYojimbo();

	protected:
		ServerYojimbo& owner;
	};
}
