
#include <iostream>

#include "Common/Core/Base.hpp"
#include "Common/Core/Core.hpp"
#include "Common/Core/ResourceManager.hpp"
#include "Common/Utility/OsAbstraction.hpp"
#include "Common/Utility/Timestep.hpp"

#include "ClientServerCommon/Plugins/Yojimbo/YojimboPlugin.hpp"

#include "DedicatedServer/DedicatedServerGame.hpp"
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

void InitLogger()
{
	const auto ClientFilename = "DedicatedServer.log";
	const auto DefaultWindowPattern = "%^[%n][%l]%$ %v";

	Logger::Initialise( "./logs/" );
	auto& logger = Logger::GetInstance();
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

	InitLogger();

	auto core = std::make_unique<Core>( GenerateCoreProperties(), std::make_unique<DedicatedServerGame>() );
	core->Init();

	const int exit_code = core->Dispatch();

	core.reset();

	return exit_code;
}
