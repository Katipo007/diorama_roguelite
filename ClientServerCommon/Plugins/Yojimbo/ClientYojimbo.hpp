#pragma once

#include "YojimboHeader.hpp"

#include "Common/Networking/Client.hpp"

namespace API
{
	class ClientYojimbo final
		: public ::Networking::Client
	{
	public:
		ClientYojimbo( yojimbo::Address&& address
			, const std::array<uint8_t, yojimbo::KeyBytes>& private_key
			, yojimbo::ClientServerConfig&& config
			, yojimbo::Adapter&& adapter
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

		yojimbo::Adapter adapter;
		yojimbo::ClientServerConfig config;
		yojimbo::Client server_connection;
	};
}
