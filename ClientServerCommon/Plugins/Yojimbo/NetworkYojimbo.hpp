#pragma once

#include <memory>
#include <string_view>

#include "Common/Core/API/NetworkAPI.hpp"

namespace Plugins
{
	class NetworkYojimbo final
		: public ::API::NetworkAPI
	{
	public:
		explicit NetworkYojimbo();
		virtual ~NetworkYojimbo();

		virtual std::string_view GetName() const noexcept override;

		std::unique_ptr<::Networking::Client> CreateClient( ::Networking::ClientProperties&& properties ) override;
		std::unique_ptr<::Networking::Server> CreateServer( ::Networking::ServerProperties&& properties ) override;

	protected:
		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Update( const PreciseTimestep& ts ) override;
	};
}
