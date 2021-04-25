#include "Core.hpp"

#include "ResourceManager.hpp"
#include "AbstractGame.hpp"

#include "Common/Core/API/BaseAPI.hpp"
#include "Common/Core/API/DearImGuiAPI.hpp"
#include "Common/Core/API/InputAPI.hpp"
#include "Common/Core/API/SystemAPI.hpp"
#include "Common/Core/API/VideoAPI.hpp"

Core::Core( std::unique_ptr<AbstractGame> _game, PluginFactoryMap_T& plugin_factories )
	: game( std::move( _game ) )
	, target_fps( 60 )
{
	if (!game)
		throw std::runtime_error( "Where yo game at? (Game object was null at Core construction)" );

	///
	/// initialise plugins
	///
	{
		API::SystemAPI* system = nullptr;
		API::VideoAPI* video = nullptr;
		for (auto i = 0; i < static_cast<size_t>(API::APIType::NumAPITypes); i++)
		{
			const auto api_t = static_cast<API::APIType>(i);
			if (plugin_factories.count( api_t ) > 0)
			{
				const auto& factory = plugin_factories[api_t];
				apis[i].reset( factory( system, video ) );
				ASSERT( apis[i] != nullptr );

				LOG_INFO( Application, "Added '{}'", apis[i]->GetName() );
				
				if (api_t == API::APIType::System)
					system = dynamic_cast<API::SystemAPI*>( apis[i].get() );
				else if (api_t == API::APIType::Video)
					video = dynamic_cast<API::VideoAPI*>(apis[i].get());
			}
		}

		if (!system)
			FATAL( "No system interface was provided" );
	}
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
				DoVariableUpdate( PreciseTimestep( current_time_seconds, std::min( seconds_since_last_frame, Max_DeltaTimeSeconds ) ) );
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

	is_initialised = false;
}

void Core::DoFixedUpdate( const PreciseTimestep& ts )
{
	if (is_running)
		game->OnFixedUpdate( ts );
}

void Core::DoVariableUpdate( const PreciseTimestep& ts )
{
	PumpEvents( ts );
	const auto& dearimgui_api = GetAPI<API::DearImGuiAPI>();
	if( dearimgui_api )
		dearimgui_api->OnFrameBegin();

	if (is_running)
		game->OnVariableUpdate( ts );

	if (dearimgui_api)
		dearimgui_api->OnFrameEnd();

	if (const auto& system_api = GetAPI<API::SystemAPI>())
		system_api->Update( ts );

	DoRender( ts );
}

void Core::DoRender( const PreciseTimestep& ts )
{
	if (const auto& video_api = GetAPI<API::VideoAPI>())
	{
		video_api->BeginRender();

		if (is_running)
			game->OnRender( ts );

		if (const auto& dearimgui_api = GetAPI<API::DearImGuiAPI>())
			dearimgui_api->DoRender();

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

	std::for_each( std::begin( apis ), std::end( apis ), []( const std::unique_ptr<API::BaseAPI>& entry )
		{
			if( entry )
				entry->Init();
		} );
}

void Core::ShutdownAPIs()
{
	// order is important and should be done in reverse of that in InitAPIs()

	std::for_each( std::rbegin( apis ), std::rend( apis ), []( const std::unique_ptr<API::BaseAPI>& entry )
		{
			if( entry )
				entry->Shutdown();
		} );
}
