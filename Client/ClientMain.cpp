
#include <iostream>

#include "Visual/Plugins/OpenGL/VideoOpenGL.hpp"
#include "Visual/Plugins/SDL2/SystemSDL2.hpp"
#include "Visual/Plugins/SDL2/InputSDL2.hpp"
#include "Visual/Plugins/DearImGUI/DearImGuiPlugin.hpp"
#include "Visual/Graphics/Texture.hpp"
#include "Visual/Resources/SpriteSheet.hpp"

#include "Common/Core/Base.hpp"
#include "Common/Core/Core.hpp"
#include "Common/Core/ResourceManager.hpp"
#include "Common/Utility/OsAbstraction.hpp"
#include "Common/Utility/Timestep.hpp"

#include "ClientServerCommon/Plugins/Yojimbo/YojimboPlugin.hpp"

#include "Client/Plugins/PluginTypes.hpp"
#include "Client/ClientGame.hpp"

CoreProperties GenerateCoreProperties()
{
	CoreProperties props;
	props.fps = 60;
	props.max_plugins = ClientPlugins::NumClientPlugins;

	props.plugin_factory = []( Core& core, APIType type ) -> std::unique_ptr<API::BaseAPI>
	{
		switch (type)
		{
		case CoreAPIs::System: return std::make_unique<Graphics::API::SystemSDL2>();
		case CoreAPIs::Input: return std::make_unique<Plugins::InputSDL2>( core.GetRequiredAPI<API::SystemAPI>() );
		case CoreAPIs::Video: return std::make_unique<Graphics::API::VideoOpenGL>( core.GetRequiredAPI<API::SystemAPI>() );
#if (DEVELOPER_TOOLS == 1)
		case CoreAPIs::DearImGui: return std::make_unique<Graphics::API::DearImGuiPlugin>( core.GetRequiredAPI<API::SystemAPI>(), core.GetRequiredAPI<API::VideoAPI>() );
#endif

		case ClientServerCommonPlugins::Yojimbo: return std::make_unique<Plugins::YojimboPlugin>();

		default: return nullptr;
		}
	};

	props.resource_initaliser_func = []( ResourceManager& manager )
	{
		manager.Init<Graphics::Texture>();
		manager.Init<Graphics::SpriteSheet>();
		manager.Init<Graphics::Sprite>();
	};

	return props;
}

void InitLogger()
{
	const auto ClientFilename = "Client.log";
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

	logger.AddSink( LoggingChannels::Client,
		{
			.name = "Client",
			.output_filename = ClientFilename,
			.window_output_pattern = DefaultWindowPattern,
#if _DEBUG
			.level = Logger::Level::Trace,
#endif
		} );

	logger.AddSink( LoggingChannels::OpenGL,
		{
			.name = "OpenGL",
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

	auto core = std::make_unique<Core>( GenerateCoreProperties(), std::make_unique<ClientGame>() );
	core->Init();
	int exit_code = core->Dispatch();
	core.reset();

	return exit_code;
}
