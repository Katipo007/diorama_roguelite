#pragma once

#include <memory>
#include <string_view>

#include "Common/Utility/Signal.hpp"
#include "Common/Core/API/BaseAPI.hpp"
#include "ClientServerCommon/Plugins/PluginTypes.hpp"

namespace YojimboPlugin
{
	class BaseServer;
	class BaseClient;
}

namespace Plugins
{
	class YojimboPlugin final
		: public API::BaseAPI
	{
	public:
		explicit YojimboPlugin();
		~YojimboPlugin();

		static constexpr APIType GetType() noexcept { return ClientServerCommonPlugins::Yojimbo; }
		std::string_view GetName() const noexcept override { return "Yojimbo"; }

		void Add( ::YojimboPlugin::BaseServer& server );
		void Add( ::YojimboPlugin::BaseClient& client );
		void Remove( ::YojimboPlugin::BaseServer& server );
		void Remove( ::YojimboPlugin::BaseClient& client );

	protected:
		void Init() override;
		void Shutdown() override;
		void OnVariableUpdate( const PreciseTimestep& ts, const StepType type ) override;

	protected:
		bool initialised = false;
		std::vector<::YojimboPlugin::BaseClient*> clients;
		std::vector<::YojimboPlugin::BaseServer*> servers;
	};
}
