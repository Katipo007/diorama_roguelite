
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

#include "ClientServerCommon/Plugins/Yojimbo/NetworkYojimbo.hpp"

#include "Client/ClientGame.hpp"

std::unique_ptr<Core> core;

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
	{
		// TODO: swap plugins based on system

		using APIFactory_T = std::function<API::BaseAPI* ( API::SystemAPI*, API::VideoAPI*)>;
		std::unordered_map<API::APIType, APIFactory_T> plugin_factories = {
			{ API::APIType::System, []( API::SystemAPI*, API::VideoAPI* ) { return new Graphics::API::SystemSDL2(); } },
			{ API::APIType::Video, []( API::SystemAPI* system, API::VideoAPI* ) { ASSERT( system ); return new Graphics::API::VideoOpenGL( *system ); } },
			{ API::APIType::Network, []( API::SystemAPI* system, API::VideoAPI* ) { ASSERT( system ); return new Plugins::NetworkYojimbo(); } },

#if (DEVELOPER_TOOLS == 1)
			{ API::APIType::DearImGui, []( API::SystemAPI* system, API::VideoAPI* video ) { ASSERT( system && video ); return new Graphics::API::DearImGuiPlugin( *system, *video ); } },
#endif
		};

		const auto resource_initaliser = []( ResourceManager& manager )
		{
			manager.Init<Graphics::Texture>();
			manager.Init<Graphics::SpriteSheet>();
			manager.Init<Graphics::Sprite>();
		};

		core = std::make_unique<Core>( std::make_unique<Game::ClientGame>(), resource_initaliser, plugin_factories );
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

	LOG_INFO( Application, "Client finished" );
	return exit_code;
}
