#pragma once

#include <memory>
#include <string_view>

#include "Common/Core/API/BaseAPI.hpp"
#include "ClientServerCommon/Plugins/PluginTypes.hpp"

namespace Plugins
{
	class YojimboPlugin final
		: public API::BaseAPI
	{
	public:
		explicit YojimboPlugin();
		virtual ~YojimboPlugin();

		static constexpr APIType GetType() noexcept { return ClientServerCommonPlugins::Yojimbo; }
		virtual std::string_view GetName() const noexcept override { return "Yojimbo"; }

	protected:
		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Update( const PreciseTimestep& ts ) override;
	};
}
