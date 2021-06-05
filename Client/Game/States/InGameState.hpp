#pragma once

#include <memory>

#include "Client/Graphical/UI/ChatWindow.hpp"
#include "Common/Utility/StateMachine/DefaultAction.hpp"
#include "Common/Utility/StateMachine/Might.hpp"
#include "Common/Utility/StateMachine/TransitionTo.hpp"
#include "Common/Utility/NonCopyable.hpp"
#include "Events.hpp"

namespace Visual
{
	class Camera;
	class SphericalCamera;
}

namespace Game::States
{
	class MainMenuState;
	class DisconnectedFromServerState;

	class InGameState
		: public fsm::DefaultAction<fsm::NoAction>
		, NonCopyable
	{
	public:
		using fsm::DefaultAction<fsm::NoAction>::HandleEvent;

		InGameState();
		~InGameState();

		void AddChatMessage( std::string_view sender, std::string_view message );

	public: // state machine stuffs

		void OnLeave();

		fsm::NoAction HandleEvent( const Events::FrameEvent& e );
		fsm::NoAction HandleEvent( const Events::RenderEvent& e );
		fsm::NoAction HandleEvent( const Events::DearImGuiFrameEvent& e );
		fsm::TransitionTo<DisconnectedFromServerState> HandleEvent( const Events::DisconnectedFromServerEvent& e );

	protected:

		void ChatWindowSendMessageHandler( std::string_view msg );

	protected:
		Networking::ClientServerSession* client_server_session = nullptr;

		std::shared_ptr<Visual::SphericalCamera> main_camera;
		UI::ChatWindow chat_window;
	};
}
