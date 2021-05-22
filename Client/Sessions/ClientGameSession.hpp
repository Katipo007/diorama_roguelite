#pragma once

#include "BaseClientSession.hpp"
#include "Common/Utility/Signal.hpp"

namespace Sessions
{
	// A connection to a server for which we are in the "in-game" state for (i.e. playing)
	class ClientGameSession
		: public BaseClientSession
	{
	public:
		ClientGameSession( std::unique_ptr<Networking::ClientServer::ServerConnection>& connection );
		virtual ~ClientGameSession();

	public: // Signals

		Signal::signal<std::string_view> ChatMessageReceived;

	protected:
		bool MessageHandler( Networking::ClientServer::ServerConnection& connection, const yojimbo::Message& message ) override;
	};
}
