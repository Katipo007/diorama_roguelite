#include "JoinMultiplayerState.hpp"

#include "Client/ClientGame.hpp"
#include "Client/Networking/ClientServer/ServerConnection.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"

#include "Visual/DearImGui/DearImGui.hpp"

namespace ClientStates
{
	JoinMultiplayerState::JoinMultiplayerState( Game::ClientGame& _client )
		: client( _client )
	{
	}

	JoinMultiplayerState::JoinMultiplayerState( JoinMultiplayerState&& to_move ) = default;

	JoinMultiplayerState::~JoinMultiplayerState() = default;

	JoinMultiplayerState::ExitActions JoinMultiplayerState::HandleEvent( const FrameEvent& )
	{
		return fsm::Actions::NoAction{};
	}

	JoinMultiplayerState::ExitActions JoinMultiplayerState::HandleEvent( const DearImGuiFrameEvent& e )
	{
		(void)e;

		if (ImGui::Begin( "JoinMultiplayerState", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove ))
		{
			ImGui::Text( "Join Multiplayer" );

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
				client.ConnectToServer( address_textbox_value );

			if (ImGui::Button( "Connect" ))
				client.ConnectToServer( address_textbox_value );

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

	JoinMultiplayerState::ExitActions JoinMultiplayerState::HandleEvent( const ConnectedToServerEvent& e )
	{
		if (&e.connection == &client.GetServerConnection())
			return fsm::Actions::TransitionTo<ConnectingToServerState>{};
		else
			return fsm::Actions::NoAction{};
	}

	JoinMultiplayerState::ExitActions JoinMultiplayerState::HandleEvent( const ConnectingToServerEvent& e )
	{
		if (&e.connection == &client.GetServerConnection())
			return fsm::Actions::TransitionTo<ConnectingToServerState>{};
		else
			return fsm::Actions::NoAction{};
	}
}
