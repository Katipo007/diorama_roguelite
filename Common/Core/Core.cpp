#include "Core.hpp"

#include "ResourceManager.hpp"
#include "AbstractGame.hpp"

Core::Core( std::unique_ptr<AbstractGame> _game )
	: game( std::move( _game ) )
	, target_fps( 60 )
{
	if (!game)
		throw std::runtime_error( "Where yo game at? (Game object was null at Core construction)" );
}

Core::~Core()
{
	Shutdown();
}

void Core::Init()
{
	ASSERT( !is_initialised );

	InitResources();
	InitRNG();

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
	resource_manager.reset();

	is_initialised = false;
}
