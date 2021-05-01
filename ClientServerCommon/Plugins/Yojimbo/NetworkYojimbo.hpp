#pragma once

#include <memory>
#include "Common/Core/API/NetworkAPI.hpp"

namespace API
{
	class ClientYojimbo;
	class ServerYojimbo;

	class NetworkYojimbo final
		: public NetworkAPI
	{
	public:
		explicit NetworkYojimbo();
		virtual ~NetworkYojimbo();

		virtual std::string_view GetName() const noexcept override;

		std::unique_ptr<::Networking::Client> CreateClient( const ::Networking::ClientProperties& properties ) override;
		std::unique_ptr<::Networking::Server> CreateServer( const ::Networking::ServerProperties& properties ) override;

	protected:
		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Update( const PreciseTimestep& ts ) override;

	protected:
	};
}
