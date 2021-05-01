#pragma once

#include "APITypesEnum.hpp"
#include "BaseAPI.hpp"

namespace Networking
{
	class Client;
	struct ClientProperties;
	class Server;
	struct ServerProperties;
}

namespace API
{
	class NetworkAPI
		: public BaseAPI
	{
		friend class ::Core;

	public:
		virtual ~NetworkAPI() {}

		static constexpr APIType GetType() noexcept { return APIType::Network; }

		virtual std::unique_ptr<::Networking::Client> CreateClient( const ::Networking::ClientProperties& properties ) = 0;
		virtual std::unique_ptr<::Networking::Server> CreateServer( const ::Networking::ServerProperties& properties ) = 0;

	private:
		virtual void Update( const PreciseTimestep& ts ) = 0;
	};
}
