#pragma once

#include <memory>
#include <type_traits>
#include <queue>

#include "Common/Core/AbstractGame.hpp"
#include "Common/Utility/NonCopyable.hpp"
#include "Common/Utility/Timestep.hpp"
#include "Common/Networking/Address.hpp"

class Core;
class ResourceManager;

namespace API { class DearImGuiAPI; }
namespace Networking { class Client; }

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
		Networking::Client* GetClientServerSession();
		Networking::Client* GetClientServerSession() const;
		void ConnectToServer( const ::Networking::Address& address );
		void DisconnectFromServer();

	protected:
		virtual void Init() override;
		virtual void OnGameEnd() override;

		virtual void OnFixedUpdate( const PreciseTimestep& ts ) override;
		virtual void OnVariableUpdate( const PreciseTimestep& ts ) override;
		virtual void OnRender( const PreciseTimestep& ts ) override;
		void DoDearImGuiFrame();

		void ConnectionStateChangedHandler( Networking::Client& sender );

	protected:
		std::unique_ptr<Networking::Client> client_server_session;

		::API::DearImGuiAPI* dearimgui = nullptr;

	private:
		struct ClientData;
		std::unique_ptr<ClientData> client_data;
	};
}
