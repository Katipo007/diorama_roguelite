#pragma once

#include "ClientServerCommon/Vendor/Wrappers/Networking.hpp"
#include "ClientServerCommon/Networking/ClientServerConfig.hpp"

#include "Common/Utility/Timestep.hpp"

namespace Sessions
{
	// Represents the client-side connection to a server.
	// Handles all communication with the server.
	class ClientServerSession
	{
	public:
		enum class ConnectionState { Disconnected, Connecting, Connected };
		struct ConnectionAdapter final : public yojimbo::Adapter
		{
			yojimbo::MessageFactory* CreateMessageFactory( yojimbo::Allocator& allocator ) override;
		};

	public:
		explicit ClientServerSession( const yojimbo::Address& address );
		virtual ~ClientServerSession();

		ConnectionState GetConnectionState() const;

		void Update( const PreciseTimestep& timestep );
		void Disconnect();

	protected:

	private:
		void ProcessMessages();
		void ProcessMessage( yojimbo::Message* message );

		bool disconnect_asap = false;

		ConnectionAdapter adaptor;
		Networking::ClientServerGameConnectionConfig connection_config;
		yojimbo::Client server_connection;
	};
}
