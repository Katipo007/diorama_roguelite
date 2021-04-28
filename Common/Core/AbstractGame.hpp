#pragma once

#include <optional>
#include "Common/Utility/Timestep.hpp"

class Core;
class ResourceManager;

class AbstractGame
{
	friend class Core;

public:
	virtual ~AbstractGame();

	void Exit( int exit_code );

protected:
	virtual void Init() = 0;
	virtual void OnGameEnd() = 0;
	const std::optional<int>& GetExitCode() const noexcept { return application_exit_code; }

	virtual void OnFixedUpdate( const PreciseTimestep& ts ) = 0;
	virtual void OnVariableUpdate( const PreciseTimestep& ts ) = 0;
	virtual void OnRender( const PreciseTimestep& ts ) = 0;

protected:
	Core* core = nullptr;
	ResourceManager* resource_manager = nullptr;

private:
	std::optional<int> application_exit_code = std::nullopt;
};
