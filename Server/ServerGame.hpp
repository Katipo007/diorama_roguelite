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
	class Server;
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

		YojimboPlugin::Server* GetServer();
		const YojimboPlugin::Server* GetServer() const;

	protected:
		void Init() override;
		void OnGameEnd() override;

		void OnFixedUpdate( const PreciseTimestep& ts ) override;
		void OnVariableUpdate( const PreciseTimestep& ts ) override;
		void OnRender( const PreciseTimestep& ) override {}

		void ClientConnectedHandler( YojimboPlugin::Server&, YojimboPlugin::ClientConnection& );
		void ClientDisconnectedHandler( YojimboPlugin::Server&, YojimboPlugin::ClientConnection& );

	private:
		std::unique_ptr<YojimboPlugin::Server> server;

		struct Data; // doing this so we don't pollute namespace of things which need to reference ServerGame
		std::unique_ptr<Data> data;
	};
}
