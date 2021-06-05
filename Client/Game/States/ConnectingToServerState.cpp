#include "ConnectingToServerState.hpp"

#include "Client/Game/Networking/ClientServerSession.hpp"
#include "Common/Utility/StringUtility.hpp"

#include "Visual/DearImGui/DearImGui.hpp"

namespace Game::States
{
	fsm::NoAction ConnectingToServerState::OnEnter( const Events::ConnectingToServerEvent& e )
	{
		client_server_session = &e.session;
		return fsm::NoAction();
	}

	void ConnectingToServerState::OnLeave()
	{
		client_server_session = nullptr;
	}

	fsm::NoAction ConnectingToServerState::HandleEvent( const Events::DearImGuiFrameEvent& )
	{
		if (ImGui::Begin( "ConnectingToServerState", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove ))
		{
			std::string_view status_message = "<ERROR>";

			if (client_server_session)
			{
				status_message = "Connecting to server...";

				if (client_server_session->IsConnected())
					status_message = "Waiting for login...";
				else if (client_server_session->IsDisconnected())
					status_message = "Disconnected.";
			}

			ImGui::Text( status_message.data() );

			if (client_server_session)
			{
				if (ImGui::Button( "Cancel" ))
					client_server_session->Disconnect();
			}

			ImGui::End();
		}

		return fsm::NoAction{};
	}

	fsm::TransitionTo<DisconnectedFromServerState> ConnectingToServerState::HandleEvent( const Events::DisconnectedFromServerEvent& )
	{
		return fsm::TransitionTo<DisconnectedFromServerState>{};
	}
}
