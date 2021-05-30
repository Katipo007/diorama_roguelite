#pragma once

#include <memory>
#include <optional>
#include <type_traits>
#include <queue>

#include "Common/Core/AbstractGame.hpp"
#include "Common/Utility/NonCopyable.hpp"
#include "Common/Utility/Timestep.hpp"

class Core;
class ResourceManager;

namespace API { class DearImGuiAPI; }
namespace yojimbo { class Message; }
namespace Networking::ClientServer { class ServerConnection; }

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

		Networking::ClientServer::ServerConnection& GetServerConnection();
		const Networking::ClientServer::ServerConnection& GetServerConnection() const;
		bool IsConnectedToServer() const noexcept;

		std::string_view GetUsername() const;

		void ConnectToServer( std::string_view address, std::string_view username );
		void DisconnectFromServer( std::optional<std::string> reason = std::nullopt );

	protected:
		virtual void Init() override;
		virtual void OnGameEnd() override;

		virtual void OnFixedUpdate( const PreciseTimestep& ts ) override;
		virtual void OnVariableUpdate( const PreciseTimestep& ts ) override;
		virtual void OnRender( const PreciseTimestep& ts ) override;
		void DoDearImGuiFrame();

		void ServerConnectionStateChangedHandler( Networking::ClientServer::ServerConnection& connection );
		bool ServerConnectionMessageHandler( Networking::ClientServer::ServerConnection& connection, const yojimbo::Message& message );

	protected:
		::API::DearImGuiAPI* dearimgui = nullptr;

	private:
		struct Pimpl;
		std::unique_ptr<Pimpl> pimpl;
	};
}
