#pragma once

#include <string>
#include <vector>

#include "Common/Core/API/APITypesEnum.hpp"
#include "Common/Utility/Timestep.hpp"

class AbstractGame;
class ResourceManager;

namespace API
{
	class InputAPI;
	class InternalInputAPI;
	class SystemAPI;
	class InternalSystemAPI;
	class VideoAPI;
	class InternalVideoAPI;

	class InternalAPI;
}

/// <summary>
/// The be all container for the application
/// </summary>
class Core final
{
public:
	using PluginFactory_T = std::function<API::InternalAPI* ( API::SystemAPI* )>;
	using PluginFactoryMap_T = std::unordered_map<API::APIType, PluginFactory_T>;

public:
	explicit Core( std::unique_ptr<AbstractGame> game, PluginFactoryMap_T& plugin_factory );
	~Core();

	void Init();

	AbstractGame& GetGame() const { return *game; }
	ResourceManager& GetResourceManager() const { return *resource_manager; }

	API::InputAPI* Input = nullptr;
	API::SystemAPI* System = nullptr;
	API::VideoAPI* Video = nullptr;

	int Dispatch();

private:
	void InitResources();
	void InitRNG();

	void Shutdown();

	void DoFixedUpdate( const PreciseTimestep& ts );
	void DoVariableUpdate( const PreciseTimestep& ts );
	void DoRender( const PreciseTimestep& ts );

	void PumpEvents();

	void InitAPIs();
	void AssignAPIs();
	void ShutdownAPIs();

private:
	bool is_initialised = false;

	int target_fps;
	std::unique_ptr<AbstractGame> game;
	std::unique_ptr<ResourceManager> resource_manager;

	std::unique_ptr<API::InternalInputAPI> input_api;
	std::unique_ptr<API::InternalSystemAPI> system_api;
	std::unique_ptr<API::InternalVideoAPI> video_api;
};
