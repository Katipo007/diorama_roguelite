#pragma once

#include "Common/Networking/Client.hpp"

#include "YojimboHeader.hpp"
#include "YojimboNetworkAdapters.hpp"

namespace Networking { class IMessageFactory; }

namespace Plugins { class NetworkYojimbo; }

namespace Plugins::Network::Yojimbo
{
	class ClientYojimbo final
		: public ::Networking::Client
	{
		friend class ::Plugins::NetworkYojimbo;

	public:
		ClientYojimbo(
			yojimbo::Address&& address
			, const std::array<uint8_t, yojimbo::KeyBytes>& private_key
			, yojimbo::ClientServerConfig&& config
			, ClientAdapter&& adapter
		);
		~ClientYojimbo();

		ConnectionState GetState() const noexcept override;

		void Disconnect() override;

	private:
		void Update( const PreciseTimestep& timestep ) override;

		void ProcessMessages();

	private:
		bool wants_to_disconnect = false;
		yojimbo::ClientState previous_connection_state = yojimbo::ClientState::CLIENT_STATE_DISCONNECTED;

		ClientAdapter adapter;
		yojimbo::ClientServerConfig config;
		yojimbo::Client server_connection;
	};
}
