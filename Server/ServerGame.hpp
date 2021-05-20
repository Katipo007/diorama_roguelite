#pragma once

#include <memory>
#include <type_traits>
#include <queue>

#include "Common/Core/AbstractGame.hpp"
#include "Common/Utility/NonCopyable.hpp"
#include "Common/Utility/Timestep.hpp"

class Core;
class ResourceManager;

namespace YojimboPlugin
{
	class BaseServer;
	class ClientConnection;
}

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

		YojimboPlugin::BaseServer* GetServer();
		const YojimboPlugin::BaseServer* GetServer() const;

	protected:
		void Init() override;
		void OnGameEnd() override;

		void OnFixedUpdate( const PreciseTimestep& ts ) override;
		void OnVariableUpdate( const PreciseTimestep& ts ) override;
		void OnRender( const PreciseTimestep& ) override {}

		void ClientConnectedHandler( YojimboPlugin::BaseServer&, YojimboPlugin::ClientConnection& );
		void ClientDisconnectedHandler( YojimboPlugin::BaseServer&, YojimboPlugin::ClientConnection& );

	private:
		std::unique_ptr<YojimboPlugin::BaseServer> server;

		struct Data; // doing this so we don't pollute namespace of things which need to reference ServerGame
		std::unique_ptr<Data> data;
	};
}
