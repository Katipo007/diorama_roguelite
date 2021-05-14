#pragma once

#include <array>
#include <string>
#include <vector>

#include "Common/Core/API/CoreAPIsEnum.hpp"
#include "Common/Utility/Timestep.hpp"

class AbstractGame;
class ResourceManager;

namespace API
{
	class BaseAPI;
	class SystemAPI;
	class VideoAPI;
}

/// <summary>
/// The be all container for the application
/// </summary>
class Core final
{
public:
	using ResourceManagerInitaliserFunc_T = std::function<void( ResourceManager& )>;
	using PluginFactoryFunc_T = std::function<std::unique_ptr<API::BaseAPI>( Core&, APIType )>;

public:
	explicit Core( std::unique_ptr<AbstractGame> game, const ResourceManagerInitaliserFunc_T& resource_initaliser_func, const PluginFactoryFunc_T& plugin_factory );
	~Core();

	void Init();

	AbstractGame& GetGame() const { return *game; }
	ResourceManager& GetResourceManager() const { return *resource_manager; }

	template<class API_T>
	API_T* GetAPI() noexcept { return const_cast<API_T*>(((const Core*)this)->GetAPI<API_T>()); }

	template<class API_T>
	const API_T* GetAPI() const noexcept
	{
		constexpr auto type = API_T::GetType();
		static_assert(std::is_same<decltype(type), const APIType>::value, "API_T::GetType() return type must be APIType");
		return dynamic_cast<const API_T*>(GetAPI( type ));
	}

	template<class API_T>
	API_T& GetRequiredAPI() noexcept { return const_cast<API_T&>(((const Core*)this)->GetRequiredAPI<API_T>()); }

	template<class API_T>
	const API_T& GetRequiredAPI() const noexcept
	{
		constexpr auto type = API_T::GetType();
		static_assert(std::is_same<decltype(type), const APIType>::value, "API_T::GetType() return type must be APIType");
		return dynamic_cast<const API_T&>(GetRequiredAPI( type ));
	}

	inline API::BaseAPI* GetAPI( const APIType type ) noexcept { return apis[type].get(); }
	inline const API::BaseAPI* GetAPI( const APIType type ) const noexcept { return apis[type].get(); }
	inline API::BaseAPI& GetRequiredAPI( const APIType type ) { if (auto* api = GetAPI( type )) return *api; throw std::runtime_error( "Missing required API" ); }
	inline const API::BaseAPI& GetRequiredAPI( const APIType type ) const { if (auto* api = GetAPI( type )) return *api; throw std::runtime_error( "Missing required API" ); }

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

	ResourceManagerInitaliserFunc_T resource_initaliser_func;

	std::vector<std::unique_ptr<API::BaseAPI>> apis;
};
