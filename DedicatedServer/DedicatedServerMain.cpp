
#include <iostream>

#include "Common/Core/Base.hpp"
#include "Common/Core/Core.hpp"
#include "Common/Core/ResourceManager.hpp"
#include "Common/Utility/OsAbstraction.hpp"
#include "Common/Utility/Timestep.hpp"

#include "ClientServerCommon/Plugins/Yojimbo/YojimboPlugin.hpp"

#include "DedicatedServer/ServerGame.hpp"
#include "DedicatedServer/Plugins/PluginTypes.hpp"
#include "DedicatedServer/Plugins/CLI/SystemCLI.hpp"

CoreProperties GenerateCoreProperties()
{
	CoreProperties props;
	props.fps = 0;
	props.max_plugins = DedicatedServerPlugins::NumDedicatedServerPlugins;

	props.plugin_factory = []( Core& core, APIType type ) -> std::unique_ptr<API::BaseAPI>
	{
		(void)core;

		switch (type)
		{
		case CoreAPIs::System: return std::make_unique<Plugins::SystemCLI>();
		case ClientServerCommonPlugins::Yojimbo: return std::make_unique<Plugins::YojimboPlugin>();

		default: return nullptr;
		}
	};

	props.resource_initaliser_func = []( ResourceManager& manager )
	{
		(void)manager;
		// TODO
	};

	return props;
}

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
	auto core = std::make_unique<Core>( GenerateCoreProperties(), std::make_unique<Game::ServerGame>() );
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
