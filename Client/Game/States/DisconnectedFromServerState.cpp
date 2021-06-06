#include "DisconnectedFromServerState.hpp"

#include "Client/Game/Networking/ClientServerSession.hpp"

#include "Visual/DearImGUI/DearImGui.hpp"

namespace Game::States
{
	fsm::NoAction DisconnectedFromServerState::OnEnter( const Events::DisconnectedFromServerEvent& e )
	{
		given_reason = e.session.GetDisconnectionReason();
		return fsm::NoAction();
	}

	void DisconnectedFromServerState::OnLeave()
	{
		given_reason.reset();
	}

	fsm::Might<fsm::TransitionTo<JoinMultiplayerState>> DisconnectedFromServerState::HandleEvent( const Events::DearImGuiFrameEvent& )
	{
		if (ImGui::Begin( "DisconnectedFromServerState", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove ))
		{
			ImGui::Text( "Disconnected from server" );

			if (given_reason.has_value())
				ImGui::Text( "Reason: %s", given_reason.value().c_str() );

			if (ImGui::Button( "Back" ))
			{
				ImGui::End();
				return fsm::TransitionTo<JoinMultiplayerState>();
			}

			ImGui::End();
		}

		return fsm::NoAction{};
	}
}
