
#include <iostream>

#include "Common/Core/Base.hpp"
#include "Common/Core/Core.hpp"
#include "Common/Core/ResourceManager.hpp"
#include "Common/Utility/OsAbstraction.hpp"
#include "Common/Utility/Timestep.hpp"

#include "ClientServerCommon/Plugins/Yojimbo/YojimboPlugin.hpp"

#include "Server/ServerGame.hpp"
#include "Server/Plugins/CLI/SystemCLI.hpp"

int main( int argc, char** argv )
{
	// TODO: use application parameters
	(void)argc;
	(void)argv;

	Logging::InitDefaultServerSinks();

	LOG_INFO( Application, "Dedicated server starting" );


	// ============================================
	// construct core
	// ============================================
	const Core::PluginFactoryFunc_T PluginFactory = []( Core& core, APIType type ) -> std::unique_ptr<API::BaseAPI>
	{
		(void)core;

		// TODO: swap plugins based on system
		switch (type)
		{
		case CoreAPIs::System: return std::make_unique<Plugins::SystemCLI>();
		case ClientServerCommonPlugins::Yojimbo: return std::make_unique<Plugins::YojimboPlugin>();

		default: return nullptr;
		}
	};
	static_assert(std::is_convertible<Core::PluginFactoryFunc_T, decltype(PluginFactory)>::value, "Plugin factory type is not compatable");
	
	const auto ResourceInitaliser = []( ResourceManager& manager )
	{
		(void)manager;
		// TODO
	};

	auto core = std::make_unique<Core>( std::make_unique<Game::ServerGame>(), ResourceInitaliser, PluginFactory );
	core->Init();

	// ============================================
	// main loop
	// ============================================
	const int exit_code = core->Dispatch();

	// ============================================
	// cleanup
	// ============================================
	core.reset();

	LOG_INFO( Application, "Dedicated server finished" );
	return exit_code;
}
