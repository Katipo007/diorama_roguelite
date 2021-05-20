#pragma once

#include <memory>
#include <type_traits>
#include <queue>

#include "Common/Core/AbstractGame.hpp"
#include "Common/Utility/NonCopyable.hpp"
#include "Common/Utility/Timestep.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/Types.hpp"

class Core;
class ResourceManager;

namespace API { class DearImGuiAPI; }

namespace YojimboPlugin { class BaseClient; }

namespace Game
{
	class ClientGame final
		: public AbstractGame
		, NonCopyable
	{
		friend class ::Core;

	public:
		explicit ClientGame();
		~ClientGame();

		Core& GetCore() const { return *core; }
		ResourceManager& GetResourceManager() const { return *resource_manager; }

		//
		// Client server session management
		//
		YojimboPlugin::BaseClient* GetClientServerSession();
		YojimboPlugin::BaseClient* GetClientServerSession() const;
		void ConnectToServer( const YojimboPlugin::Address& address );
		void DisconnectFromServer();

	protected:
		virtual void Init() override;
		virtual void OnGameEnd() override;

		virtual void OnFixedUpdate( const PreciseTimestep& ts ) override;
		virtual void OnVariableUpdate( const PreciseTimestep& ts ) override;
		virtual void OnRender( const PreciseTimestep& ts ) override;
		void DoDearImGuiFrame();

		void ConnectionStateChangedHandler( YojimboPlugin::BaseClient& sender );

	protected:
		std::unique_ptr<YojimboPlugin::BaseClient> client_server_session;

		::API::DearImGuiAPI* dearimgui = nullptr;

	private:
		struct ClientData;
		std::unique_ptr<ClientData> client_data;
	};
}
