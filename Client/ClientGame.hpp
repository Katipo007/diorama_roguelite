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
namespace Game::Networking { class ClientServerSession; }

class ClientGame final
	: public AbstractGame
	, NonCopyable
{
public:
	explicit ClientGame();
	~ClientGame();

	Core& GetCore() const { return *core; }
	ResourceManager& GetResourceManager() const { return *resource_manager; }

	Game::Networking::ClientServerSession* GetClientServerSession() noexcept { return client_server_session.get(); }
	const Game::Networking::ClientServerSession* GetClientServerSession() const noexcept { return client_server_session.get(); }
	bool IsConnectedToServer() const noexcept { return GetClientServerSession() != nullptr; }

	void ConnectToServer( std::string_view address );
	void DisconnectFromServer();

protected:
	virtual void Init() override;
	virtual void OnGameEnd() override;

	virtual void OnFixedUpdate( const PreciseTimestep& ts ) override;
	virtual void OnVariableUpdate( const PreciseTimestep& ts ) override;
	virtual void OnRender( const PreciseTimestep& ts ) override;
	void DoDearImGuiFrame();

	void ServerConnectionStateChangedHandler( Game::Networking::ClientServerSession& );

protected:
	::API::DearImGuiAPI* dearimgui = nullptr;

private:
	std::unique_ptr<Game::Networking::ClientServerSession> client_server_session;
	struct Pimpl;
	std::unique_ptr<Pimpl> pimpl;
};
