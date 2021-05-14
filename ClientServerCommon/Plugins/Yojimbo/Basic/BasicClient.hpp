#pragma once

#include "../Client.hpp"
#include "BasicAdapter.hpp"

#include "../impl/YojimboHeader.hpp"

namespace Plugins { class YojimboPlugin; }

namespace YojimboPlugin
{
	class BasicClient final
		: public Client
	{
	public:
		BasicClient(
			const Address& address
			, const std::array<uint8_t, yojimbo::KeyBytes>& private_key
			, yojimbo::ClientServerConfig&& config
			, BasicAdapter&& adapter
		);
		~BasicClient();

		ConnectionState GetState() const noexcept override;

		void Disconnect() override;

	public: // events
		sigslot::signal<Client&> ConnectionStateChanged;

	private:
		void Update( const PreciseTimestep& timestep ) override;

		void ProcessMessages();

	private:
		bool wants_to_disconnect = false;
		yojimbo::ClientState previous_connection_state = yojimbo::ClientState::CLIENT_STATE_DISCONNECTED;

		const yojimbo::ClientServerConfig config;
		BasicAdapter adapter;
		yojimbo::Client server_connection;
	};
}
