#include "DisconnectedFromServerState.hpp"

#include "Visual/DearImGUI/DearImGui.hpp"

namespace ClientStates
{
	fsm::Actions::NoAction DisconnectedFromServerState::OnEnter( const DisconnectedFromServerState& e )
	{
		given_reason = e.given_reason;
		return fsm::Actions::NoAction();
	}

	void DisconnectedFromServerState::OnLeave()
	{
		given_reason.clear();
	}

	fsm::Actions::Might<fsm::Actions::TransitionTo<JoinMultiplayerState>> DisconnectedFromServerState::HandleEvent( const DearImGuiFrameEvent& )
	{
		if (ImGui::Begin( "DisconnectedFromServerState", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove ))
		{
			ImGui::Text( "Disconnected from server" );

			if (!given_reason.empty())
				ImGui::Text( "Reason: %s", given_reason.c_str() );

			if (ImGui::Button( "Back" ))
			{
				ImGui::End();
				return fsm::Actions::TransitionTo<JoinMultiplayerState>();
			}

			ImGui::End();
		}

		return fsm::Actions::NoAction{};
	}
}
