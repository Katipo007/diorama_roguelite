#pragma once

#include <memory>
#include <type_traits>
#include <queue>

#include "Common/Core/AbstractGame.hpp"
#include "Common/Utility/NonCopyable.hpp"
#include "Common/Utility/Timestep.hpp"

class Core;
class ResourceManager;

namespace yojimbo
{
	class Address;
}

namespace Sessions
{
	class ClientServerSession;
}

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

		void Exit();

		ResourceManager& GetResourceManager() const { return *resource_manager; }

		//
		// Client server session management
		//
		Sessions::ClientServerSession* GetClientServerSession() { return client_server_session.get(); }
		const Sessions::ClientServerSession* GetClientServerSession() const { return client_server_session.get(); }
		void ConnectToServer( const yojimbo::Address& address );
		void DisconnectFromServer();

	protected:
		virtual void Init() override;
		virtual void OnGameEnd() override;

		virtual void OnFixedUpdate( const PreciseTimestep& ts ) override;
		virtual void OnVariableUpdate( const PreciseTimestep& ts ) override;
		virtual void OnRender( const PreciseTimestep& ts ) override;
		void DoDearImGuiFrame();

		void ConnectionStateChangedHandler( Sessions::ClientServerSession& sender );

	protected:
		std::unique_ptr<Sessions::ClientServerSession> client_server_session;
		Core* core = nullptr;
		ResourceManager* resource_manager = nullptr;

		bool user_requested_exit = false;

	private:
		struct ClientData;
		std::unique_ptr<ClientData> client_data;
	};
}
