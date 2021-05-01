#pragma once

#include <memory>
#include <type_traits>
#include <queue>

#include "Common/Core/AbstractGame.hpp"
#include "Common/Utility/NonCopyable.hpp"
#include "Common/Utility/Timestep.hpp"

class Core;
class ResourceManager;

namespace Game
{
	class ServerGame final
		: public AbstractGame
		, NonCopyable
	{
		friend class ::Core;

	public:
		explicit ServerGame();
		~ServerGame();

		Core& GetCore() const { return *core; }
		ResourceManager& GetResourceManager() const { return *resource_manager; }

	protected:
		virtual void Init() override;
		virtual void OnGameEnd() override;

		virtual void OnFixedUpdate( const PreciseTimestep& ts ) override;
		virtual void OnVariableUpdate( const PreciseTimestep& ts ) override;
		virtual void OnRender( const PreciseTimestep& ) override {}

	private:
		struct ServerData;
		std::unique_ptr<ServerData> data;
	};
}
