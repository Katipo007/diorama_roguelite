#include "InGameState.hpp"

#include <functional>

#include "Client/ClientGame.hpp"

#include "Common/Core/Core.hpp"
#include "Common/Core/API/VideoAPI.hpp"

#include "Visual/Camera.hpp"

namespace Game::States
{
	InGameState::InGameState()
	{
		main_camera = std::make_shared<Visual::SphericalCamera>();
		main_camera->SetPosition( { 0, 0, 0 } );
		main_camera->SetAspectRatio( 1.f );
		main_camera->SetClipSpace( 0.01f, 600.f );
		main_camera->SetFOV( 75.f );
		main_camera->SetRadius( 128.f );
		main_camera->SetRotationD( 95.f, 0.f );
	}

	InGameState::~InGameState()
	{
	}

	//fsm::Might<fsm::TransitionTo<DisconnectedFromServerState>>
	fsm::NoAction InGameState::OnEnter()
	{
		ASSERT( game );
		client_server_session = game->GetClientServerSession();
		ASSERT( client_server_session );
		//if (!client_server_session)
		//	return fsm::TransitionTo<DisconnectedFromServerState>{};

		chat_window.SetClientServerSession( client_server_session );

		return fsm::NoAction{};
	}

	void InGameState::OnLeave()
	{
		chat_window.SetClientServerSession( nullptr );
		client_server_session = nullptr;
	}

	fsm::NoAction InGameState::HandleEvent( const Events::FrameEvent& )
	{
		return fsm::NoAction{};
	}

	fsm::NoAction InGameState::HandleEvent( const Events::RenderEvent& )
	{
		// scene->Render();

		return fsm::NoAction();
	}

	fsm::NoAction InGameState::HandleEvent( const Events::DearImGuiFrameEvent& )
	{
		chat_window.Do();

		return fsm::NoAction{};
	}

	fsm::TransitionTo<DisconnectedFromServerState> InGameState::HandleEvent( const Events::DisconnectedFromServerEvent& )
	{
		return fsm::TransitionTo<DisconnectedFromServerState>{};
	}

	void InGameState::AddChatMessage( std::string_view sender, std::string_view message )
	{
		using namespace std::string_literals;
		std::string to_display = "["s + sender.data() + "] " + message.data();
		chat_window.AddMessage( to_display );
	}

	void States::InGameState::SetGameReference( ClientGame& game_ )
	{
		game = &game_;
	}
}
