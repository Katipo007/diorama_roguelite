#include "JoinMultiplayerState.hpp"

#include "Client/ClientGame.hpp"
#include "Visual/DearImGui/DearImGui.hpp"

namespace ClientStates
{
	JoinMultiplayerState::JoinMultiplayerState( Game::ClientGame& _client )
		: client( _client )
	{
	}

	JoinMultiplayerState::~JoinMultiplayerState()
	{
	}

	JoinMultiplayerState::ExitActions JoinMultiplayerState::HandleEvent( const FrameEvent& e )
	{
		(void)e;

		// TODO: update status message with current connection state

		return fsm::Actions::NoAction{};
	}

	JoinMultiplayerState::ExitActions JoinMultiplayerState::HandleEvent( const DearImGuiFrameEvent& e )
	{
		(void)e;

		if (ImGui::Begin( "JoinMultiplayerState", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove ))
		{
			ImGui::Text( "Join Multiplayer" );

			// TODO: check if we are connecting
			const bool is_connecting = false; // client.GetClientServerSession() != nullptr;
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
				CancelConnection();
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
		NOT_IMPLEMENTED;
	}

	void JoinMultiplayerState::CancelConnection()
	{
		NOT_IMPLEMENTED;
	}
}
