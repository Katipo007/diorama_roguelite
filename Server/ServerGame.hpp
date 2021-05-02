#pragma once

#include <memory>
#include <type_traits>
#include <queue>

#include "Common/Core/AbstractGame.hpp"
#include "Common/Utility/NonCopyable.hpp"
#include "Common/Utility/Timestep.hpp"

class Core;
class ResourceManager;

namespace Networking { class Server; }

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

		::Networking::Server* GetServer();
		const ::Networking::Server* GetServer() const;

	protected:
		virtual void Init() override;
		virtual void OnGameEnd() override;

		virtual void OnFixedUpdate( const PreciseTimestep& ts ) override;
		virtual void OnVariableUpdate( const PreciseTimestep& ts ) override;
		virtual void OnRender( const PreciseTimestep& ) override {}

	private:
		std::unique_ptr<Networking::Server> server;

		struct Data; // doing this so we don't pollute namespace of things which need to reference ServerGame
		std::unique_ptr<Data> data;
	};
}
