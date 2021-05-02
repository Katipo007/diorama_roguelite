
#include <iostream>

#include "Common/Core/Base.hpp"
#include "Common/Core/Core.hpp"
#include "Common/Core/ResourceManager.hpp"
#include "Common/Utility/OsAbstraction.hpp"
#include "Common/Utility/Timestep.hpp"

#include "ClientServerCommon/Plugins/Yojimbo/NetworkYojimbo.hpp"

#include "Server/ServerGame.hpp"
#include "Server/Plugins/CLI/SystemCLI.hpp"

std::unique_ptr<Core> core;

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
	{
		// TODO: swap plugins based on system

		using APIFactory_T = std::function<API::BaseAPI* (API::SystemAPI*, API::VideoAPI*)>;
		std::unordered_map<API::APIType, APIFactory_T> plugin_factories = {
			{ API::APIType::System, []( API::SystemAPI*, API::VideoAPI* ) { return new Plugins::SystemCLI(); } },
			{ API::APIType::Network, []( API::SystemAPI* system, API::VideoAPI* ) { ASSERT( system ); return new Plugins::NetworkYojimbo(); } },
		};

		const auto resource_initaliser = []( ResourceManager& manager )
		{
			(void)manager;
			// TODO
		};

		core = std::make_unique<Core>( std::make_unique<Game::ServerGame>(), resource_initaliser, plugin_factories );
		core->Init();
	}

	// ============================================
	// main loop
	// ============================================
	int exit_code = 0;
	{
		exit_code = core->Dispatch();
	}

	// ============================================
	// cleanup
	// ============================================
	{
		core.reset();
	}

	LOG_INFO( Application, "Dedicated server finished" );
	return exit_code;
}
