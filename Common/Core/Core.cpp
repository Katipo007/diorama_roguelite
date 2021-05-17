#include "Core.hpp"

#include "ResourceManager.hpp"
#include "AbstractGame.hpp"

#include "Common/Core/API/BaseAPI.hpp"
#include "Common/Core/API/DearImGuiAPI.hpp"
#include "Common/Core/API/InputAPI.hpp"
#include "Common/Core/API/SystemAPI.hpp"
#include "Common/Core/API/VideoAPI.hpp"

#include "Common/Core/Resources/StandardResources.hpp"

Core::Core( CoreProperties&& props, std::unique_ptr<AbstractGame> game_ )
	: game( std::move( game_ ) )
	, resource_initaliser_func( props.resource_initaliser_func )
	, target_fps( std::max( 0, props.fps ) )
{
	ASSERT( props.IsValid() );
	if (!props.IsValid())
		throw std::runtime_error( "Bad core properties" );

	if (!game)
		throw std::runtime_error( "Where yo game at? (Game object was null at Core construction)" );

	///
	/// initialise plugins
	///
	apis.resize( props.max_plugins );
	for (APIType t = 0; t < props.max_plugins; t++)
	{
		auto plugin = props.plugin_factory( *this, t );
		if (plugin != nullptr)
			apis[t] = std::move( plugin );
	}
	LOG_INFO( Application, "{} plugins initalised", std::count_if( std::begin( apis ), std::end( apis ), []( const auto& entry ) { return entry != nullptr; } ) );
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

	game->core = this;
	game->resource_manager = resource_manager.get();
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

	if (target_fps <= 0)
	{
		while (is_running)
		{
			const auto current_time = Clock_T::now();
			const double current_time_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(current_time.time_since_epoch()).count() / 1000.0;
			constexpr double fixed_delta = 1.0 / 60.0;

			const auto timestep = PreciseTimestep( current_time_seconds, fixed_delta );
			DoFixedUpdate( timestep );
			DoVariableUpdate( timestep );
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
					DoFixedUpdate( PreciseTimestep( current_time_seconds, fixed_timestep_seconds ) );

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
				if (seconds_since_last_frame > 0)
					DoVariableUpdate( PreciseTimestep( current_time_seconds, std::min( seconds_since_last_frame, Max_DeltaTimeSeconds ) ) );
				else
					LOG_WARN( Application, "Skipped DoVariableUpdate: delta-time was 0" );
			}

			last_time = current_time;
		}
	}

	return 0;
}

void Core::InitResources()
{
	ASSERT( !resource_manager );
	resource_manager = std::make_unique<ResourceManager>( *this );

	Resources::Standard::InitStandardResources( *resource_manager );

	if (resource_initaliser_func)
		resource_initaliser_func( *resource_manager );
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

	is_initialised = false;
}

void Core::DoFixedUpdate( const PreciseTimestep& ts )
{
	assert( ts.delta > 0 );

	for (auto& plugin : active_apis)
		plugin->OnFixedUpdate( ts, StepType::PreGameStep );

	if (is_running)
		game->OnFixedUpdate( ts );

	for (auto& plugin : active_apis)
		plugin->OnFixedUpdate( ts, StepType::PostGameStep );
}

void Core::DoVariableUpdate( const PreciseTimestep& ts )
{
	assert( ts.delta > 0 );
	PumpEvents( ts );

	for (auto& plugin : active_apis)
		plugin->OnVariableUpdate( ts, StepType::PreGameStep );

	if (game->GetExitCode())
	{
		exit_code = game->GetExitCode().value();
		is_running = false;
	}

	if (is_running)
		game->OnVariableUpdate( ts );

	for (auto& plugin : active_apis)
		plugin->OnVariableUpdate( ts, StepType::PostGameStep );

	DoRender( ts );
}

void Core::DoRender( const PreciseTimestep& ts )
{
	if (const auto& video_api = GetAPI<API::VideoAPI>())
	{
		video_api->BeginRender();

		for (auto& plugin : active_apis)
			plugin->OnRender( ts, StepType::PreGameStep );

		if (is_running)
			game->OnRender( ts );

		for (auto& plugin : active_apis)
			plugin->OnRender( ts, StepType::PostGameStep );

		video_api->EndRender();
	}
}

void Core::PumpEvents( const PreciseTimestep& ts )
{
	const auto& system_api = GetAPI<API::SystemAPI>();
	const auto& input_api = GetAPI<API::InputAPI>();
	const auto& video_api = GetAPI<API::VideoAPI>();
	const auto& dearimgui_api = GetAPI<API::DearImGuiAPI>();

	if( input_api )
		input_api->BeginEvents( ts );

	if (!system_api->GenerateEvents( video_api, input_api, dearimgui_api ))
	{
		exit_code = 0;
		is_running = false;
	}
}

void Core::InitAPIs()
{
	// order is important

	active_apis.clear();
	std::for_each( std::begin( apis ), std::end( apis ), [this]( const std::unique_ptr<API::BaseAPI>& entry )
		{
			if (entry)
			{
				entry->Init();
				active_apis.push_back( entry.get() );
			}
		} );
}

void Core::ShutdownAPIs()
{
	// order is important and should be done in reverse of that in InitAPIs()

	active_apis.clear();
	std::for_each( std::rbegin( apis ), std::rend( apis ), []( const std::unique_ptr<API::BaseAPI>& entry )
		{
			if( entry )
				entry->Shutdown();
		} );
}
