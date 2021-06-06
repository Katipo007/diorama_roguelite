#pragma once

#include <memory>
#include <type_traits>
#include <queue>

#include "Common/Core/AbstractGame.hpp"
#include "Common/Utility/NonCopyable.hpp"
#include "Common/Utility/Timestep.hpp"

class Core;
class ResourceManager;

namespace Game { class Server; }

class DedicatedServerGame final
	: public AbstractGame
	, NonCopyable
{
public:
	explicit DedicatedServerGame();
	~DedicatedServerGame();

	Core& GetCore() const { return *core; }
	ResourceManager& GetResourceManager() const { return *resource_manager; }

protected:
	void Init() override;
	void OnGameEnd() override;

	void OnFixedUpdate( const PreciseTimestep& ts ) override;
	void OnVariableUpdate( const PreciseTimestep& ts ) override;
	void OnRender( const PreciseTimestep& ) override {}

private:
	std::unique_ptr<Game::Server> server;
};
