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
		void Init() override;
		void OnGameEnd() override;

		void OnFixedUpdate( const PreciseTimestep& ts ) override;
		void OnVariableUpdate( const PreciseTimestep& ts ) override;
		void OnRender( const PreciseTimestep& ) override {}

	private:
		// TODO: game server

		struct Pimpl;
		const std::unique_ptr<Pimpl> pimpl;
	};
}
