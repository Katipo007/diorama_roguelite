#pragma once

#include <memory>

#include "Client/Graphical/UI/ChatWindow.hpp"
#include "Client/States/Events.hpp"
#include "Common/Utility/StateMachine/DefaultAction.hpp"
#include "Common/Utility/StateMachine/Might.hpp"
#include "Common/Utility/StateMachine/TransitionTo.hpp"
#include "Common/Utility/NonCopyable.hpp"

namespace API
{
	class InputAPI;
}

namespace Visual
{
	class Camera;
	class SphericalCamera;
}

namespace Game
{
	class ClientGame;
	class ClientGameWorld;
}

namespace ClientStates
{
	class MainMenuState;
	class DisconnectedFromServerState;

	class InGameState
		: public fsm::DefaultAction<fsm::NoAction>
	{
	public:
		using fsm::DefaultAction<fsm::NoAction>::HandleEvent;

		explicit InGameState( Game::ClientGame& client );
		explicit InGameState( InGameState&& to_move );
		virtual ~InGameState();

		void AddChatMessage( std::string_view sender, std::string_view message );

	public: // state machine stuffs

		fsm::Might<fsm::TransitionTo<MainMenuState>> OnEnter();

		void OnLeave();
		void OnLeave( const ClientStates::DisconnectedFromServerEvent& e );

		fsm::NoAction HandleEvent( const FrameEvent& e );
		fsm::NoAction HandleEvent( const RenderEvent& e );
		fsm::NoAction HandleEvent( const DearImGuiFrameEvent& e );
		fsm::NoAction HandleEvent( const ServerMessageEvent& e );
		fsm::TransitionTo<DisconnectedFromServerState> HandleEvent( const DisconnectedFromServerEvent& e );

	protected:
		void OnRender() const;

		void ChatWindowSendMessageHandler( std::string_view msg );

	protected:
		Game::ClientGame& game;

		API::InputAPI& input;

		UI::ChatWindow chat_window;
		std::shared_ptr<Visual::SphericalCamera> main_camera;
		std::unique_ptr<Game::ClientGameWorld> gameworld; // TODO: refactor into client session

	private:
		InGameState( const InGameState& ) = delete;
		InGameState operator=( const InGameState& ) = delete;
	};
}
