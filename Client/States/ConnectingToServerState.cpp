#include "ConnectingToServerState.hpp"

#include "Client/Networking/ClientServer/ServerConnection.hpp"
#include "ClientServerCommon/Networking/ClientServer/Config.hpp"

#include "Common/Utility/StringUtility.hpp"

#include "Visual/DearImGui/DearImGui.hpp"

namespace ClientStates
{
	fsm::Actions::NoAction ConnectingToServerState::OnEnter( const ConnectingToServerEvent& )
	{
		return fsm::Actions::NoAction();
	}

	fsm::Actions::Might<fsm::Actions::TransitionTo<JoinMultiplayerState>> ConnectingToServerState::HandleEvent( const DearImGuiFrameEvent& )
	{
		if (ImGui::Begin( "ConnectingToServerState", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove ))
		{
			ImGui::Text( "Connecting to server..." );

			if (ImGui::Button( "Cancel" ))
			{
				ImGui::End();
				
				return fsm::Actions::TransitionTo<JoinMultiplayerState>();
			}

			ImGui::End();
		}

		return fsm::Actions::NoAction{};
	}

	fsm::Actions::Might<fsm::Actions::TransitionTo<InGameState>> ConnectingToServerState::HandleEvent( const ServerMessageEvent& )
	{
		return fsm::Actions::NoAction{};
	}

	fsm::Actions::NoAction ConnectingToServerState::HandleEvent( const ConnectedToServerEvent& e )
	{
		using namespace Networking::ClientServer;
		e.connection.SendMessage<Messages::ClientServerChatMessage>( ChannelType::Reliable, []( Messages::ClientServerChatMessage& msg )
			{
				StringUtility::StringToArray( "Test", msg.message );
			} );

		return fsm::Actions::NoAction();
	}

	fsm::Actions::TransitionTo<DisconnectedFromServerState> ConnectingToServerState::HandleEvent( const DisconnectedFromServerEvent& )
	{
		return fsm::Actions::TransitionTo<DisconnectedFromServerState>();
	}
}
