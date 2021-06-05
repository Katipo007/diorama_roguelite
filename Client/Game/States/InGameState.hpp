#pragma once

#include <memory>

#include "Client/Graphical/UI/ChatWindow.hpp"
#include "Common/Utility/StateMachine/DefaultAction.hpp"
#include "Common/Utility/StateMachine/Might.hpp"
#include "Common/Utility/StateMachine/TransitionTo.hpp"
#include "Common/Utility/NonCopyable.hpp"
#include "Events.hpp"

class ClientGame;

namespace Visual
{
	class Camera;
	class SphericalCamera;
}

namespace Game::States
{
	class DisconnectedFromServerState;

	class InGameState final
		: public fsm::DefaultAction<fsm::NoAction>
		, NonCopyable
	{
	public:
		using fsm::DefaultAction<fsm::NoAction>::HandleEvent;

		explicit InGameState();
		~InGameState();

		void AddChatMessage( std::string_view sender, std::string_view message );

	public: // state machine stuffs

		fsm::NoAction OnEnter();
		void OnLeave();

		fsm::NoAction HandleEvent( const Events::FrameEvent& e );
		fsm::NoAction HandleEvent( const Events::RenderEvent& e );
		fsm::NoAction HandleEvent( const Events::DearImGuiFrameEvent& e );
		fsm::TransitionTo<DisconnectedFromServerState> HandleEvent( const Events::DisconnectedFromServerEvent& e );

	protected:
		ClientGame* game = nullptr;
		Networking::ClientServerSession* client_server_session = nullptr;

		std::shared_ptr<Visual::SphericalCamera> main_camera;
		UI::ChatWindow chat_window;
	};
}
