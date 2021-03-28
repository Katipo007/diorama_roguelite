
#include <iostream>

#include "Common/Core/Base.hpp"
#include "Common/Utility/OsAbstraction.hpp"
#include "Common/Utility/Timestep.hpp"

#include "Server/GameServer.hpp"

std::unique_ptr<Server::GameServer> game_server;

bool running = true;
bool user_wants_to_exit = false;

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

	LOG_INFO( Server, "[yojimbo] {}", buffer );
	return 0;
}

int main( int argc, char** argv )
{
	// TODO: use application parameters
	(void)argc;
	(void)argv;

	Logging::InitDefaultServerSinks();

	LOG_INFO( Application, "Dedicated server starting" );

	//
	// Setup Yojimbo
	//
	{
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

	//
	// Setup
	//
	game_server.reset( new Server::GameServer( yojimbo::Address( "127.0.0.1", 42777 ) ) ); // TODO: allow setting bind address/port and NumMaxClients via commandline


	// ============================================
	// main loop
	// ============================================

	LOG_INFO( Application, "Dedicated server application complete, starting main loop" );

	constexpr size_t TargetFramesPerSecond = 30;
	static double next_step = yojimbo_time();
	constexpr double FixedDt = 1.0 / TargetFramesPerSecond;

	constexpr size_t MaxStepsPerFrame = 10;
	size_t steps_this_frame = 0;
	while (running)
	{
		double current_time = yojimbo_time();
		if (steps_this_frame >= MaxStepsPerFrame)
		{
			LOG_WARN( Server, "Max steps per frame exceeded, is the server overloaded?" );
			next_step = current_time + FixedDt;
			yojimbo_sleep( next_step - current_time );
			steps_this_frame = 0;
		}
		else if (next_step <= current_time )
		{
			// do event
			PreciseTimestep timestep( current_time, FixedDt );
			game_server->Update( timestep );

			// track time
			next_step += FixedDt;
			++steps_this_frame;

			// exit
			if (!game_server->IsRunning())
			{
				running = false;
				continue;
			}
		}
		else
		{
			yojimbo_sleep( next_step - current_time );
			steps_this_frame = 0;
		}
	}

	LOG_INFO( Application, "Dedicated application loop finished, exiting" );

	//
	// clear
	//
	{
		game_server.reset();
	}

	//
	// Shutdown Yojimbo
	//
	ShutdownYojimbo();

	LOG_INFO( Application, "Dedicated server finished" );
	return 0;
}
