#pragma once

#include "Common/Utility/Timestep.hpp"

class Core;
class ResourceManager;

class AbstractGame
{
	friend class Core;

public:
	virtual ~AbstractGame() {}

protected:
	virtual void Init() = 0;
	virtual void OnGameEnd() = 0;

	virtual void OnFixedUpdate( const PreciseTimestep& ts ) = 0;
	virtual void OnVariableUpdate( const PreciseTimestep& ts ) = 0;
	virtual void OnRender( const PreciseTimestep& ts ) = 0;

protected:
	Core* core = nullptr;
	ResourceManager* resource_manager = nullptr;
};
