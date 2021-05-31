#include "MainMenuState.hpp"

#include "Client/ClientGame.hpp"
#include "Common/Version/BuildVersion.hpp"
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

	MainMenuState::OutTransitionActions MainMenuState::HandleEvent( const DearImGuiFrameEvent& )
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
			return fsm::Actions::TransitionTo<JoinMultiplayerState>{};
		else if( do_exit )
			client.Exit( 0 );

		return fsm::Actions::NoAction{};
	}
}
