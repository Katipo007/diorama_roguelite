#pragma once

#include "../Client.hpp"
#include "BasicAdapter.hpp"

#include "../impl/YojimboHeader.hpp"

namespace Plugins { class YojimboPlugin; }

namespace YojimboPlugin
{
	// A basic client implementation.
	// Auto-registers itself with the plugin
	class BasicClient final
		: public BaseClient
	{
	public:
		struct Definition
		{
			Plugins::YojimboPlugin* plugin = nullptr;

			Address binding_address = "0.0.0.0";
			Address target_address = {};
			Key_T private_key = { 0 };
			yojimbo::ClientServerConfig config;
			BasicAdapter adapter;
		};

	public:
		BasicClient( Definition&& );
		~BasicClient();

		ConnectionState GetState() const noexcept override;

		void SendMessage( const MessageType type, const ChannelType channel, const std::function<void( Message& )>& initialiser );

		void Disconnect() override;

	private:
		void Update( const PreciseTimestep& timestep ) override;

		void ProcessMessages();

	private:
		bool wants_to_disconnect = false;
		yojimbo::ClientState previous_connection_state = yojimbo::ClientState::CLIENT_STATE_DISCONNECTED;

		Definition definition;
		yojimbo::Client server_connection;
	};
}
