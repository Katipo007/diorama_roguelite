#include "JoinMultiplayerState.hpp"

#include "Client/ClientGame.hpp"
#include "Client/Game/Networking/ClientIdGenerator.hpp"

#include "Common/Game/Character/CharacterUtility.hpp"
#include "Common/Utility/Unreachable.hpp"

#include "Visual/DearImGui/DearImGui.hpp"

namespace
{
	std::array<char, 128> address_textbox_value{ "127.0.0.1:42777" };
	const auto address_textbox_handler = []( ImGuiInputTextCallbackData* ) -> int { return 0; };

	std::array<char, Game::CharacterUtility::CharacterNameMaxLength> username_textbox_value{ "Player" };
	const auto username_textbox_handler = []( ImGuiInputTextCallbackData* ) -> int { return 0; };
}

namespace Game::States
{
	JoinMultiplayerState::ExitActions JoinMultiplayerState::HandleEvent( const Events::FrameEvent& )
	{
		return fsm::NoAction{};
	}

	JoinMultiplayerState::ExitActions JoinMultiplayerState::HandleEvent( const Events::DearImGuiFrameEvent& e )
	{
		(void)e;

		ImGui::SetNextWindowSize( ImVec2( 600, 400 ) );
		if (ImGui::Begin( "JoinMultiplayerState", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove ))
		{
			ImGui::Text( "Join Multiplayer" );

			ImGui::InputText( "IP Address", &address_textbox_value[0], std::size( address_textbox_value ), ImGuiInputTextFlags_EnterReturnsTrue, address_textbox_handler );
			ImGui::InputText( "Username", &username_textbox_value[0], std::size( username_textbox_value ), ImGuiInputTextFlags_EnterReturnsTrue, address_textbox_handler );

			if (ImGui::Button( "Connect" ))
				ConnectToServer();

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

	JoinMultiplayerState::ExitActions JoinMultiplayerState::HandleEvent( const Events::ConnectedToServerEvent& )
	{
		FATAL( "We shouldn't be able to get here" );
		//return fsm::TransitionTo<ConnectingToServerState>{};
	}

	JoinMultiplayerState::ExitActions JoinMultiplayerState::HandleEvent( const Events::ConnectingToServerEvent& )
	{
		return fsm::TransitionTo<ConnectingToServerState>{};
	}

	void States::JoinMultiplayerState::ConnectToServer()
	{
		Networking::ServerConnectionRequest request
		{
			.destination{ address_textbox_value.data() },
			.username{ username_textbox_value.data() },
			.client_id{ Networking::GenerateClientId() },
		};
		ConnectToServerClicked( std::move( request ) );
	}
}
