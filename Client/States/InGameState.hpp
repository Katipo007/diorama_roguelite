#pragma once

#include <memory>

#include "Client/Graphical/UI/ChatWindow.hpp"
#include "Client/States/Events.hpp"
#include "Common/Utility/StateMachine/DefaultAction.hpp"
#include "Common/Utility/StateMachine/Actions/Might.hpp"
#include "Common/Utility/StateMachine/Actions/TransitionTo.hpp"
#include "Common/Utility/NonCopyable.hpp"

namespace Visual
{
	class Camera;
	class SphericalCamera;
}

namespace Game
{
	class ClientGame;
	class PlayerObject;
	class ClientGameWorld;
}

namespace Sessions
{
	class ClientServerSession;
}

namespace ClientStates
{
	class MainMenuState;

	class InGameState
		: public fsm::DefaultAction<fsm::Actions::NoAction>
	{
	public:
		using fsm::DefaultAction<fsm::Actions::NoAction>::HandleEvent;

		explicit InGameState( Game::ClientGame& client );
		explicit InGameState( InGameState&& to_move );
		virtual ~InGameState();

		fsm::Actions::Might<fsm::Actions::TransitionTo<MainMenuState>> OnEnter();
		//fsm::Actions::Might<fsm::Actions::TransitionTo<MainMenuState>> OnEnter( const FrameEvent& ) { LOG_INFO( Client, "Entered via FrameEvent!" ); return OnEnter(); }

		fsm::Actions::TransitionTo<MainMenuState> OnLeave();
		fsm::Actions::TransitionTo<MainMenuState> OnLeave( const ClientStates::DisconnectedFromServerEvent& e );

		fsm::Actions::NoAction HandleEvent( const FrameEvent& e );
		fsm::Actions::NoAction HandleEvent( const RenderEvent& e );
		fsm::Actions::NoAction HandleEvent( const DearImGuiFrameEvent& e );
		fsm::Actions::TransitionTo<MainMenuState> HandleEvent( const DisconnectedFromServerEvent& e );

	protected:
		void OnRender() const;

		void ChatWindowSendMessageHandler( std::string_view msg );
		void ChatMessageReceivedHandler( const std::string& sender, const std::string& message );

		Game::ClientGame& client;
		Sessions::ClientServerSession* client_server_session = nullptr;

		UI::ChatWindow chat_window;
		std::shared_ptr<Visual::SphericalCamera> main_camera;
		std::unique_ptr<Game::ClientGameWorld> gameworld; // TODO: refactor into client session

	private:
		InGameState( const InGameState& ) = delete;
		InGameState operator=( const InGameState& ) = delete;
	};
}
