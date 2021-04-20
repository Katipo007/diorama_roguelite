#pragma once

#include <string>
#include <vector>

#include "Common/Utility/Timestep.hpp"

class AbstractGame;
class ResourceManager;

/// <summary>
/// The be all container for the application
/// </summary>
class Core final
{
public:
	explicit Core( std::unique_ptr<AbstractGame> game );
	~Core();

	void Init();

	AbstractGame& GetGame() const { return *game; }
	ResourceManager& GetResourceManager() const { return *resource_manager; }

	int Dispatch();

private:
	void InitResources();
	void InitRNG();
	void Shutdown();

	void DoFixedUpdate( const PreciseTimestep& ts );
	void DoVariableUpdate( const PreciseTimestep& ts );
	void DoRender( const PreciseTimestep& ts );

	void PumpEvents();

private:
	bool is_initialised = false;

	int target_fps;
	std::unique_ptr<AbstractGame> game;
	std::unique_ptr<ResourceManager> resource_manager;
};
