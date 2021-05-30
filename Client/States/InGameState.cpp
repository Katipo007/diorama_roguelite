#include "InGameState.hpp"

#include <functional>

#include "Client/ClientGame.hpp"
#include "Client/Game/ClientGameWorld.hpp"
#include "Client/Game/PlayerObject.hpp"
#include "Client/Networking/ClientServer/ServerConnection.hpp"
#include "ClientServerCommon/Networking/ClientServer/Channels.hpp"

#include "Common/Core/Core.hpp"
#include "Common/Core/API/VideoAPI.hpp"
#include "Common/Utility/StringUtility.hpp"

#include "Visual/Camera.hpp"

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
		// TODO: return to menu if we don't have a connection
		//if (!client.GetClientServerSession())
		//{
		//	LOG_ERROR( Client, "Expected to have a session when entering InGameState!" );
		//	//return fsm::Actions::TransitionTo<MainMenuState>{};
		//}

		//
		// initalise
		//
		auto& video = client.GetCore().GetRequiredAPI<API::VideoAPI>();
		gameworld = std::make_unique<Game::ClientGameWorld>( video, client.GetResourceManager() );

		//
		// connect to session events
		//
		// TODO: connect to chat recieved event

		return fsm::Actions::NoAction();
	}

	void InGameState::OnLeave()
	{
		gameworld.reset();
	}

	void InGameState::OnLeave( const ClientStates::DisconnectedFromServerEvent& )
	{
		OnLeave();
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

	fsm::Actions::NoAction InGameState::HandleEvent( const ServerMessageEvent& e )
	{
		using namespace Networking::ClientServer;

		switch (e.message.GetType())
		{
			case MessageFactory::GetMessageType<Messages::ServerClientChatMessage>() :
			{
				auto& chat = static_cast<const Messages::ServerClientChatMessage&>(e.message);
				AddChatMessage( chat.sender.data(), chat.message.data() );
				e.FlagAsHandled();
				break;
			}
		}

		return fsm::Actions::NoAction();
	}

	fsm::Actions::TransitionTo<DisconnectedFromServerState> InGameState::HandleEvent( const DisconnectedFromServerEvent& )
	{
		return fsm::Actions::TransitionTo<DisconnectedFromServerState>();
	}

	void InGameState::OnRender() const
	{
		gameworld->Render( main_camera );
	}

	void InGameState::ChatWindowSendMessageHandler( std::string_view chat_message )
	{
		using namespace Networking::ClientServer;
		LOG_INFO( LoggingChannels::Client, "Sending chat message '{}'", chat_message );
		client.GetServerConnection().SendMessage<Messages::ClientServerChatMessage>( ChannelType::Reliable, [&chat_message]( Messages::ClientServerChatMessage& chat )
			{
				StringUtility::StringToArray( chat_message, chat.message );
			} );
	}

	void InGameState::AddChatMessage( std::string_view sender, std::string_view message )
	{
		using namespace std::string_literals;
		std::string to_display = "["s + sender.data() + "] " + message.data();
		chat_window.AddMessage( to_display );
	}
}
