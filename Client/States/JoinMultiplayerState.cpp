#include "JoinMultiplayerState.hpp"

#include "Client/ClientGame.hpp"
#include "Client/Networking/ClientServer/ServerConnection.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"

#include "Common/Game/Character/CharacterUtility.hpp"

#include "Visual/DearImGui/DearImGui.hpp"

namespace
{
	std::array<char, 128> address_textbox_value{ "127.0.0.1:42777" };
	const auto address_textbox_handler = []( ImGuiInputTextCallbackData* ) -> int { return 0; };

	std::array<char, Game::CharacterUtility::CharacterNameMaxLength> username_textbox_value{ "Player" };
	const auto username_textbox_handler = []( ImGuiInputTextCallbackData* ) -> int { return 0; };
}

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
		return fsm::NoAction{};
	}

	JoinMultiplayerState::ExitActions JoinMultiplayerState::HandleEvent( const DearImGuiFrameEvent& e )
	{
		(void)e;

		ImGui::SetNextWindowSize( ImVec2( 600, 400 ) );
		if (ImGui::Begin( "JoinMultiplayerState", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove ))
		{
			ImGui::Text( "Join Multiplayer" );

			ImGui::InputText( "IP Address", &address_textbox_value[0], std::size( address_textbox_value ), ImGuiInputTextFlags_EnterReturnsTrue, address_textbox_handler );
			ImGui::InputText( "Username", &username_textbox_value[0], std::size( username_textbox_value ), ImGuiInputTextFlags_EnterReturnsTrue, address_textbox_handler );

			if (ImGui::Button( "Connect" ))
				client.ConnectToServer( address_textbox_value.data(), username_textbox_value.data() );

			// return to menu
			if (ImGui::Button( "Back" ))
			{
				ImGui::End();
				return fsm::TransitionTo<MainMenuState>{};
			}

			// current status message
			ImGui::TextDisabled( status_message.c_str() );

			ImGui::End();
		}

		return fsm::NoAction{};
	}

	JoinMultiplayerState::ExitActions JoinMultiplayerState::HandleEvent( const ConnectedToServerEvent& e )
	{
		if (&e.connection == &client.GetServerConnection())
			return fsm::TransitionTo<ConnectingToServerState>{};
		else
			return fsm::NoAction{};
	}

	JoinMultiplayerState::ExitActions JoinMultiplayerState::HandleEvent( const ConnectingToServerEvent& e )
	{
		if (&e.connection == &client.GetServerConnection())
			return fsm::TransitionTo<ConnectingToServerState>{};
		else
			return fsm::NoAction{};
	}
}
