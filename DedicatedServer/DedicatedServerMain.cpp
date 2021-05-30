
#include <iostream>

#include "Common/Core/Base.hpp"
#include "Common/Core/Core.hpp"
#include "Common/Core/ResourceManager.hpp"
#include "Common/Core/Plugins/Spdlog/SpdlogPlugin.hpp"
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
		case CoreAPIs::Logging: return std::make_unique<Plugins::SpdlogPlugin>( "./Logs" );
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

void InitLogger()
{
	const auto ClientFilename = "Client.log";
	const auto DefaultWindowPattern = "%^[%n][%l]%$ %v";

	auto& logger = API::LoggingAPI::GetInstance();
	logger.AddSink( LoggingChannels::Assertion,
		{
			.name = "Assertions",
			.output_filename = ClientFilename,
			.window_output_pattern = "%^[ASSERT]%$ %v (%@)",
			.file_output_pattern = "[%T][ASSERT] %v (%@)",
		} );

	logger.AddSink( LoggingChannels::Application,
		{
			.name = "Application",
			.output_filename = ClientFilename,
			.window_output_pattern = DefaultWindowPattern,
		} );

	logger.AddSink( LoggingChannels::Server,
		{
			.name = "Server",
			.output_filename = ClientFilename,
			.window_output_pattern = DefaultWindowPattern,
		} );
}

int main( int argc, char** argv )
{
	// TODO: use application parameters
	(void)argc;
	(void)argv;

	auto core = std::make_unique<Core>( GenerateCoreProperties(), std::make_unique<Game::ServerGame>() );
	InitLogger();
	core->Init();

	const int exit_code = core->Dispatch();

	core.reset();

	return exit_code;
}
