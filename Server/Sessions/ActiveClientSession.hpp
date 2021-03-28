#pragma once

#include <bitset>
#include <optional>
#include <string>

#include "ClientServerCommon/Vendor/Wrappers/Networking.hpp"
#include "Common/Utility/NonCopyable.hpp"
#include "Common/Utility/Timestep.hpp"
#include "Common/Vendor/enumclassbitset/EnumClassBitset.hpp"

namespace Server
{
	class GameServer;
}

namespace Server::Sessions
{
	// Represents a client's connection to the server and provides
	// an interface for communicating with them
	class ActiveClientSession
		: NonCopyable
	{
	public:
		enum class Flags
		{
			None = 0,
			MarkedForDisconnection = BIT( 1 ), // If present the server will gracefully disconnect the client at the end of an update
			ServerOwner = BIT( 2 ), // If this session belongs to the server owner. Mostly used for internal server
		};

	public:
		explicit ActiveClientSession( GameServer& server, const int client_index, const uint64_t client_id );
		virtual ~ActiveClientSession();

		int GetClientIndex() const { return client_index; }
		uint64_t GetClientId() const { return client_id; }
		std::string_view GetDisplayName() const { return display_name; }

		bool IsMarkedForDisconnect() const { return (size_t)SessionFlags.value & (size_t)Flags::MarkedForDisconnection; }

		// Expected to be called by the owning server
		// Expected to be called after receiving and handling packets but before sending
		virtual void Update( const PreciseTimestep& timestep );
		virtual bool ProcessMessage( const yojimbo::Message& message );

		bitmask<Flags> SessionFlags;

	protected:
		GameServer& server;
		const int client_index;
		const uint64_t client_id;

		std::string display_name;
	};
}
ENABLE_ENUM_CLASS_BITMASK( Server::Sessions::ActiveClientSession::Flags );
