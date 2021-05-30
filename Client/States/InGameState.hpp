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

namespace ClientStates
{
	class MainMenuState;
	class DisconnectedFromServerState;

	class InGameState
		: public fsm::DefaultAction<fsm::Actions::NoAction>
	{
	public:
		using fsm::DefaultAction<fsm::Actions::NoAction>::HandleEvent;

		explicit InGameState( Game::ClientGame& client );
		explicit InGameState( InGameState&& to_move );
		virtual ~InGameState();

		void AddChatMessage( std::string_view sender, std::string_view message );

	public: // state machine stuffs

		fsm::Actions::Might<fsm::Actions::TransitionTo<MainMenuState>> OnEnter();

		void OnLeave();
		void OnLeave( const ClientStates::DisconnectedFromServerEvent& e );

		fsm::Actions::NoAction HandleEvent( const FrameEvent& e );
		fsm::Actions::NoAction HandleEvent( const RenderEvent& e );
		fsm::Actions::NoAction HandleEvent( const DearImGuiFrameEvent& e );
		fsm::Actions::NoAction HandleEvent( const ServerMessageEvent& e );
		fsm::Actions::TransitionTo<DisconnectedFromServerState> HandleEvent( const DisconnectedFromServerEvent& e );

	protected:
		void OnRender() const;

		void ChatWindowSendMessageHandler( std::string_view msg );

	protected:
		Game::ClientGame& client;

		UI::ChatWindow chat_window;
		std::shared_ptr<Visual::SphericalCamera> main_camera;
		std::unique_ptr<Game::ClientGameWorld> gameworld; // TODO: refactor into client session

	private:
		InGameState( const InGameState& ) = delete;
		InGameState operator=( const InGameState& ) = delete;
	};
}
