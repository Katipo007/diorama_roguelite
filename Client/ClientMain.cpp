
#include <iostream>

#include "Visual/Plugins/OpenGL/VideoOpenGL.hpp"
#include "Visual/Plugins/SDL2/SystemSDL2.hpp"

#include "Common/Core/Base.hpp"
#include "Common/Core/Core.hpp"
#include "Common/Utility/OsAbstraction.hpp"
#include "Common/Utility/Timestep.hpp"

#include "Client/ClientGame.hpp"
#include "ClientServerCommon/Vendor/Wrappers/Networking.hpp"

std::unique_ptr<Core> core;


static int YojimboLoggingRoute( const char* fmt, ... )
{
	char buffer[4 * 1024];
	va_list args;
	va_start( args, fmt );
	vsprintf_s( buffer, fmt, args );
	va_end( args );
	const size_t length = strlen( buffer );
	if (buffer[length - 1] == '\n')
		buffer[length - 1] = '\0';

	LOG_INFO( Client, "[yojimbo] {}", buffer );
	return 0;
}

int main( int argc, char** argv )
{
	// TODO: use application parameters
	(void)argc;
	(void)argv;

	Logging::InitDefaultClientSinks(); // TODO: move logging into a plugin

	LOG_INFO( Application, "Client starting" );

	//
	// Setup Yojimbo
	//
	{
		// TODO: move Yojimbo into a plugin
		if (!InitializeYojimbo())
		{
			LOG_CRITICAL( Client, "Critical Error: failed to initialize Yojimbo!" );
			return 1;
		}

#ifdef _DEBUG
		yojimbo_log_level( YOJIMBO_LOG_LEVEL_INFO );
#endif
		yojimbo_set_printf_function( YojimboLoggingRoute );
	}
	
	// ============================================
	// construct core
	// ============================================
	{
		// TODO: swap plugins based on system

		using APIFactory_T = std::function<API::InternalAPI* ( API::SystemAPI* )>;
		std::unordered_map<API::APIType, APIFactory_T> plugin_factories;
		plugin_factories[API::APIType::System] = []( API::SystemAPI* ) { return new Graphics::API::SystemSDL2(); };
		plugin_factories[API::APIType::Video] = []( API::SystemAPI* system ) { ASSERT( system ); return new Graphics::API::VideoOpenGL( *system ); };

		core = std::make_unique<Core>( std::make_unique<Game::ClientGame>(), plugin_factories );
		core->Init();
	}

	// ============================================
	// main loop
	// ============================================
	{
		core->Dispatch();
	}

	// ============================================
	// cleanup
	// ============================================
	{
		core.reset();
	}

	//
	// Shutdown Yojimbo
	//
	ShutdownYojimbo();

	LOG_INFO( Application, "Client finished" );
	return 0;
}
