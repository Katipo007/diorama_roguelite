#include "MainMenuState.hpp"

#include "Common/Version/BuildVersion.hpp"
#include "Visual/DearImGui/DearImGui.hpp"

namespace Game::States
{
	fsm::Might<fsm::TransitionTo<JoinMultiplayerState>, fsm::TransitionTo<ExitGameState>> MainMenuState::HandleEvent( const Events::DearImGuiFrameEvent& )
	{
		bool do_join = false;
		bool do_exit = false;

		constexpr bool use_work_area = true;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos( use_work_area ? viewport->WorkPos : viewport->Pos );
		ImGui::SetNextWindowSize( use_work_area ? viewport->WorkSize : viewport->Size );
		if (ImGui::Begin( "Main Menu", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings ))
		{
			ImGui::Text( "Main Menu" );

			ImGui::BeginGroup();
			do_join |= ImGui::Button( "Join Server" );
			do_exit |= ImGui::Button( "Exit" );
			ImGui::EndGroup();

			ImGui::BeginGroup();
			ImGui::TextDisabled( "Build %s", BuildVersion::FullVersionString.data() );
			ImGui::EndGroup();

			ImGui::End();
		}

		if( do_join )
			return fsm::TransitionTo<JoinMultiplayerState>{};
		else if( do_exit )
			return fsm::TransitionTo<ExitGameState>{};

		return fsm::NoAction{};
	}
}
