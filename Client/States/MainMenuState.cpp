#include "MainMenuState.hpp"

#include "Client/ClientGame.hpp"

#include "Visual/DearImGui/DearImGui.hpp"

namespace
{

}

namespace ClientStates
{
	MainMenuState::MainMenuState()
	{
	}

	MainMenuState::~MainMenuState()
	{
	}

	StateMachine::Actions::NoAction MainMenuState::HandleEvent( const FrameEvent& e )
	{
		(void)e;

		return StateMachine::Actions::NoAction{};
	}

	MainMenuState::OutTransitionActions MainMenuState::HandleEvent( const DearImGuiFrameEvent& e )
	{
		(void)e;

		if (ImGui::Begin( "Main Menu", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration ))
		{
			ImGui::Text( "Main Menu" );

			if (ImGui::Button( "Join Server" ))
			{
				ImGui::End();
				return StateMachine::Actions::TransitionTo<JoinMultiplayerState>{};
			}

			if (ImGui::Button( "Exit" ))
			{
				Game::GetClientGame().Exit();
			}

			ImGui::End();
		}

		return StateMachine::Actions::NoAction{};
	}
}
