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

		static bool IsInitialised() noexcept;

	protected:
		void Init() override;
		void Shutdown() override;

	protected:
		static bool s_initialised;
	};
}
