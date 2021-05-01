#include "InGameState.hpp"

#include <functional>

#include "Visual/Camera.hpp"

#include "Common/Core/Core.hpp"
#include "Common/Core/API/VideoAPI.hpp"

#include "Client/ClientGame.hpp"
#include "Client/Game/ClientGameWorld.hpp"
#include "Client/Game/PlayerObject.hpp"

namespace ClientStates
{
	InGameState::InGameState( Game::ClientGame& _client )
		: client( _client )
	{
		main_camera = std::make_shared<Visual::SphericalCamera>();
		main_camera->SetPosition( { 0, 0, 0 } );
		main_camera->SetAspectRatio( 1.f );
		main_camera->SetClipSpace( 0.01f, 600.f );
		main_camera->SetFOV( 75.f );
		main_camera->SetRadius( 128.f );
		main_camera->SetRotationD( 95.f, 0.f );

		chat_window.EnteredMessage.connect( &InGameState::ChatWindowSendMessageHandler, this );
	}

	InGameState::InGameState( InGameState&& to_move )
		: client( to_move.client )
		, chat_window( std::move( to_move.chat_window ) )
	{
		std::swap( main_camera, to_move.main_camera );
		std::swap( gameworld, to_move.gameworld );

		chat_window.EnteredMessage.disconnect( &InGameState::ChatWindowSendMessageHandler, &to_move );
		chat_window.EnteredMessage.connect( &InGameState::ChatWindowSendMessageHandler, this );
	}

	InGameState::~InGameState()
	{
		chat_window.EnteredMessage.disconnect( &InGameState::ChatWindowSendMessageHandler, this );

		gameworld.reset();
		main_camera.reset();
	}

	fsm::Actions::Might<fsm::Actions::TransitionTo<MainMenuState>> InGameState::OnEnter()
	{
		if (!client.GetClientServerSession())
		{
			LOG_ERROR( Client, "Expected to have a session when entering InGameState!" );
			client.DisconnectFromServer();
			return fsm::Actions::TransitionTo<MainMenuState>{};
		}

		//
		// initalise
		//
		auto* video = client.GetCore().GetAPI<API::VideoAPI>();
		ASSERT( video != nullptr );
		gameworld = std::make_unique<Game::ClientGameWorld>( *video, client.GetResourceManager() );

		//
		// connect to session events
		//
		// TODO: connect to chat recieved event

		return fsm::Actions::NoAction();
	}

	fsm::Actions::TransitionTo<MainMenuState> InGameState::OnLeave()
	{
		gameworld.reset();

		return fsm::Actions::TransitionTo<MainMenuState>();
	}

	fsm::Actions::TransitionTo<MainMenuState> InGameState::OnLeave( const ClientStates::DisconnectedFromServerEvent& e )
	{
		(void)e;

		return OnLeave();
	}

	fsm::Actions::NoAction ClientStates::InGameState::HandleEvent( const FrameEvent& e )
	{
		(void)e;

		return fsm::Actions::NoAction{};
	}

	fsm::Actions::NoAction InGameState::HandleEvent( const RenderEvent& e )
	{
		(void)e;

		OnRender();

		return fsm::Actions::NoAction();
	}

	fsm::Actions::NoAction ClientStates::InGameState::HandleEvent( const DearImGuiFrameEvent& e )
	{
		(void)e;

		chat_window.Do();

		return fsm::Actions::NoAction{};
	}

	fsm::Actions::TransitionTo<MainMenuState> InGameState::HandleEvent( const DisconnectedFromServerEvent& e )
	{
		(void)e;
		auto* client_server_session = client.GetClientServerSession();
		ASSERT( !client_server_session || (client_server_session == &e.session) );

		//
		// disconnect events
		//
		// TODO: disconnect chat message received event

		//
		// clear
		//
		client_server_session = nullptr;

		LOG_INFO( Client, "Lost connection to server, returning to main menu" );
		return fsm::Actions::TransitionTo<MainMenuState>();
	}

	void InGameState::OnRender() const
	{
		gameworld->Render( main_camera );
	}

	void InGameState::ChatWindowSendMessageHandler( std::string_view msg )
	{
		auto* client_server_session = client.GetClientServerSession();
		ASSERT( client_server_session );
		if (!client_server_session)
			return;

		LOG_INFO( Client, "Sending chat message '{}'", (std::string)msg );
		// TODO: send chat message
	}

	void InGameState::ChatMessageReceivedHandler( const std::string& sender, const std::string& message )
	{
		std::string to_display = "[" + sender + "] " + message;
		chat_window.AddMessage( to_display );
	}
}
