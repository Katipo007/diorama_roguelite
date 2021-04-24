#pragma once

#include <array>
#include <string>
#include <vector>

#include "Common/Core/API/APITypesEnum.hpp"
#include "Common/Utility/Timestep.hpp"

class AbstractGame;
class ResourceManager;

namespace API
{
	class BaseAPI;
	class SystemAPI;
}

/// <summary>
/// The be all container for the application
/// </summary>
class Core final
{
public:
	using PluginFactory_T = std::function<API::BaseAPI* ( API::SystemAPI* )>;
	using PluginFactoryMap_T = std::unordered_map<API::APIType, PluginFactory_T>;

public:
	explicit Core( std::unique_ptr<AbstractGame> game, PluginFactoryMap_T& plugin_factory );
	~Core();

	void Init();

	AbstractGame& GetGame() const { return *game; }
	ResourceManager& GetResourceManager() const { return *resource_manager; }

	template<class API_T> API_T* GetAPI() noexcept { constexpr API::APIType type = API_T::GetType(); return dynamic_cast<API_T*>( GetAPI( type ) ); }
	template<class API_T> const API_T* GetAPI() const noexcept { constexpr API::APIType type = API_T::GetType(); return dynamic_cast<const API_T*>(GetAPI( type )); }

	inline API::BaseAPI* GetAPI( const API::APIType type ) noexcept { return apis[(size_t)type].get(); }
	inline const API::BaseAPI* GetAPI( const API::APIType type ) const noexcept { return apis[(size_t)type].get(); }

	int Dispatch();

private:
	void InitResources();
	void InitRNG();

	void Shutdown();

	void DoFixedUpdate( const PreciseTimestep& ts );
	void DoVariableUpdate( const PreciseTimestep& ts );
	void DoRender( const PreciseTimestep& ts );

	void PumpEvents( const PreciseTimestep& ts );

	void InitAPIs();
	void ShutdownAPIs();

private:
	bool is_initialised = false;

	int target_fps;
	bool is_running = true;
	int exit_code = -1;

	std::unique_ptr<AbstractGame> game;
	std::unique_ptr<ResourceManager> resource_manager;

	std::array<std::unique_ptr<API::BaseAPI>, static_cast<size_t>(API::APIType::NumAPITypes)> apis;
};
