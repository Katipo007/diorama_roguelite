
#include <iostream>

#include "Visual/Plugins/OpenGL/VideoOpenGL.hpp"
#include "Visual/Plugins/SDL2/SystemSDL2.hpp"
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

int main( int argc, char** argv )
{
	// TODO: use application parameters
	(void)argc;
	(void)argv;

	Logging::InitDefaultClientSinks(); // TODO: move logging into a plugin

	LOG_INFO( Application, "Client starting" );
	
	// ============================================
	// construct core
	// ============================================
	auto core = std::make_unique<Core>( GenerateCoreProperties(), std::make_unique<Game::ClientGame>() );
	core->Init();

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

	LOG_INFO( Application, "Client finished" );
	return exit_code;
}
