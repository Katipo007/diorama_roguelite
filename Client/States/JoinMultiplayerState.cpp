#include "JoinMultiplayerState.hpp"

#include "Client/ClientGame.hpp"
#include "Client/Sessions/ClientServerSession.hpp"
#include "Visual/DearImGui/DearImGui.hpp"

namespace ClientStates
{
	JoinMultiplayerState::JoinMultiplayerState()
	{
	}

	JoinMultiplayerState::~JoinMultiplayerState()
	{
	}

	JoinMultiplayerState::ExitActions JoinMultiplayerState::HandleEvent( const FrameEvent& e )
	{
		(void)e;

		if (const auto* client_server_session = Game::GetClientGame().GetClientServerSession())
		{
			const auto connection_state = client_server_session->GetConnectionState();
			switch (connection_state)
			{
			case Sessions::ClientServerSession::ConnectionState::Connected:
				LOG_INFO( Client, "Connected to server" );
				status_message = "Connected";
				break;

			case Sessions::ClientServerSession::ConnectionState::Connecting:
				status_message = "Connecting...";
				break;

			case Sessions::ClientServerSession::ConnectionState::Disconnected:
				LOG_INFO( Client, "Failed to connect to server" );
				status_message = "Connection failed.";
				break;
			}
		}

		return fsm::Actions::NoAction{};
	}

	JoinMultiplayerState::ExitActions JoinMultiplayerState::HandleEvent( const DearImGuiFrameEvent& e )
	{
		(void)e;

		if (ImGui::Begin( "JoinMultiplayerState", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration ))
		{
			ImGui::Text( "Join Multiplayer" );

			const bool is_connecting = Game::GetClientGame().GetClientServerSession() != nullptr;
			if (is_connecting)
				ImGui::PushStyleVar( ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f );

			static char address_textbox_value[128] = "127.0.0.1:42777";
			const auto address_textbox_handler = []( ImGuiInputTextCallbackData* data ) -> int
			{
				(void)data;
				//switch (data->EventFlag)
				//{
				//default:
				//	// nothing
				//	break;
				//}
				return 0;
			};
			if (ImGui::InputText( "IP Address", address_textbox_value, 128, ImGuiInputTextFlags_EnterReturnsTrue, address_textbox_handler ))
			{
				if( !is_connecting )
					InitiateConnection( address_textbox_value );
			}

			if (is_connecting)
			{
				ImGui::PopStyleVar();
				if (ImGui::Button( "Cancel" ))
					CancelConnection();
			}
			else
			{
				if (ImGui::Button( "Connect" ))
					InitiateConnection( address_textbox_value );
			}

			// return to menu
			if (ImGui::Button( "Back" ))
			{
				ImGui::End();
				return fsm::Actions::TransitionTo<MainMenuState>{};
			}

			// current status message
			ImGui::TextDisabled( status_message.c_str() );

			ImGui::End();
		}

		return fsm::Actions::NoAction{};
	}

	fsm::Actions::TransitionTo<LoadingState> JoinMultiplayerState::HandleEvent( const ConnectedToServerEvent& e )
	{
		(void)e;
		return fsm::Actions::TransitionTo<LoadingState>{};
	}

	void JoinMultiplayerState::InitiateConnection( std::string address_str )
	{
		auto& game = Game::GetClientGame();
		const auto* session = game.GetClientServerSession();
		ASSERT( session == nullptr );
		if (session != nullptr)
			return;

		auto address = yojimbo::Address( address_str.c_str() );
		game.ConnectToServer( address );
	}

	void JoinMultiplayerState::CancelConnection()
	{
		auto& game = Game::GetClientGame();
		const auto* session = game.GetClientServerSession();
		if (session != nullptr)
			game.DisconnectFromServer();
	}
}
