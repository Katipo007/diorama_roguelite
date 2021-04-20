#include "MainMenuState.hpp"

#include "Client/ClientGame.hpp"

#include "Visual/DearImGui/DearImGui.hpp"

namespace
{

}

namespace ClientStates
{
	MainMenuState::MainMenuState( Game::ClientGame& _client )
		: client( _client )
	{
	}

	MainMenuState::~MainMenuState()
	{
	}

	fsm::Actions::NoAction MainMenuState::HandleEvent( const FrameEvent& e )
	{
		(void)e;

		return fsm::Actions::NoAction{};
	}

	MainMenuState::OutTransitionActions MainMenuState::HandleEvent( const DearImGuiFrameEvent& e )
	{
		(void)e;

		if (ImGui::Begin( "Main Menu", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove ))
		{
			ImGui::Text( "Main Menu" );

			if (ImGui::Button( "Join Server" ))
			{
				ImGui::End();
				return fsm::Actions::TransitionTo<JoinMultiplayerState>{};
			}

			if (ImGui::Button( "Exit" ))
			{
				client.Exit();
			}

			ImGui::End();
		}

		return fsm::Actions::NoAction{};
	}
}
