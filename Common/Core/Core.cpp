#include "Core.hpp"

#include "ResourceManager.hpp"
#include "AbstractGame.hpp"

#include "Common/Core/API/InternalAPI.hpp"

#define RETRIEVE_PLUGIN_REQUIRED( _factories, _type, _storage_ptr ) { \
	if( const auto& factory = _factories[ API::APIType::_type ] ) \
		{ _storage_ptr.reset( dynamic_cast<API::Internal##_type##API*>( factory( system_api.get() ) ) ); LOG_INFO( Application, "_type API: {}", _storage_ptr->GetName() ); } \
	if( _storage_ptr == nullptr ) \
		throw std::runtime_error( "Non-optional '" #_type "' API was not provided!" ); }

#define RETRIEVE_PLUGIN_OPTIONAL( _factories, _type, _storage_ptr ) \
	if( const auto& factory = _factories[ API::APIType::_type ] ) \
		{ _storage_ptr.reset( dynamic_cast<API::Internal##_type##API*>( factory( system_api.get() ) ) ); LOG_INFO( Application, "_type API: {}", _storage_ptr->GetName() ); } \

Core::Core( std::unique_ptr<AbstractGame> _game, PluginFactoryMap_T& plugin_factories )
	: game( std::move( _game ) )
	, target_fps( 60 )
{
	if (!game)
		throw std::runtime_error( "Where yo game at? (Game object was null at Core construction)" );


	///
	/// initialise plugins
	///
	RETRIEVE_PLUGIN_REQUIRED( plugin_factories, System, system_api );
	RETRIEVE_PLUGIN_OPTIONAL( plugin_factories, Input, input_api );
	RETRIEVE_PLUGIN_OPTIONAL( plugin_factories, Video, video_api );
}

Core::~Core()
{
	Shutdown();
}

void Core::Init()
{
	ASSERT( !is_initialised );

	InitResources();
	InitAPIs();
	InitRNG();

	AssignAPIs();
	game->Init();

	is_initialised = true;
}

int Core::Dispatch()
{
	using Clock_T = std::chrono::steady_clock;

	int64_t num_steps = 0;
	const Clock_T::time_point start_time = Clock_T::now();
	Clock_T::time_point target_time;
	Clock_T::time_point last_time;
	target_time = last_time = start_time;

	bool is_running = true;
	if (target_fps <= 0)
	{
		while (is_running)
		{
			const auto current_time = Clock_T::now();
			const double current_time_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(current_time.time_since_epoch()).count() / 1000.0;
			constexpr double fixed_delta = 1.0 / 60.0;

			const auto timestep = PreciseTimestep( current_time_seconds, fixed_delta );
			game->OnFixedUpdate( timestep );
			game->OnVariableUpdate( timestep );
		}
	}
	else
	{
		while (is_running)
		{
			Clock_T::time_point current_time = Clock_T::now();
			const double current_time_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(current_time.time_since_epoch()).count() / 1000.0;

			// Time for a fixed update?
			if (current_time >= target_time)
			{
				constexpr int MaxFixedStepsPerFrame = 5;
				const double fixed_timestep_seconds = 1.0 / target_fps;

				// Perform a given number of steps this frame
				int steps_needed = static_cast<int>(std::chrono::duration<float>( current_time - target_time ).count() * target_fps);
				for (int i = 0; i < std::min( steps_needed, MaxFixedStepsPerFrame ); i++)
					game->OnFixedUpdate( PreciseTimestep( current_time_seconds, fixed_timestep_seconds ) );

				num_steps += steps_needed;
				target_time = start_time + std::chrono::microseconds( (num_steps * 1000000ll) / target_fps );
			}
			else
			{
				std::this_thread::yield();
			}

			// Variable update
			{
				constexpr double Max_DeltaTimeSeconds = 0.1;
				const double seconds_since_last_frame = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_time).count() / 1000.0;
				game->OnVariableUpdate( PreciseTimestep( current_time_seconds, std::min( seconds_since_last_frame, Max_DeltaTimeSeconds ) ) );
			}

			last_time = current_time;
		}
	}

	return 0;
}

void Core::InitResources()
{
	resource_manager = std::make_unique<ResourceManager>();
}

void Core::InitRNG()
{
	time_t current_time = time( nullptr );
	clock_t current_clock = clock();
	int seed = static_cast<int>(current_time) ^ static_cast<int>(current_clock) ^ 0xb67b820e;
	srand( seed );
}

void Core::Shutdown()
{
	ASSERT( is_initialised );

	game->OnGameEnd();
	game.reset();

	ShutdownAPIs();
	resource_manager.reset();

	AssignAPIs();

	is_initialised = false;
}

void Core::InitAPIs()
{
	// order is important

	if (system_api)
		system_api->Init();

	if (input_api)
		input_api->Init();

	if (video_api)
		video_api->Init();
}

void Core::AssignAPIs()
{
	ASSERT( system_api != nullptr );

	System = system_api.get();
	Input = input_api.get();
	Video = video_api.get();
}

void Core::ShutdownAPIs()
{
	// order is important and should be done in reverse of that in InitAPIs()

	if (video_api)
		video_api->Shutdown();

	if (input_api)
		input_api->Shutdown();

	if (system_api)
		system_api->Shutdown();
}
