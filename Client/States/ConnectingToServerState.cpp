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

	void ConnectingToServerState::OnLeave()
	{
		if (connection != nullptr)
			DetatchFromConnection( *connection );
	}

	fsm::Actions::Might<fsm::Actions::TransitionTo<JoinMultiplayerState>> ConnectingToServerState::HandleEvent( const DearImGuiFrameEvent& )
	{
		if (ImGui::Begin( "ConnectingToServerState", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove ))
		{
			std::string_view status_message = "Connecting to server...";

			if (connection)
			{
				if (connection->IsConnected())
					status_message = "Waiting for login...";
				else if (connection->IsDisconnected())
					status_message = "Disconnected.";
			}

			ImGui::Text( status_message.data() );

			if (ImGui::Button( "Cancel" ))
			{
				ImGui::End();
				
				return fsm::Actions::TransitionTo<JoinMultiplayerState>();
			}

			ImGui::End();
		}

		return fsm::Actions::NoAction{};
	}

	fsm::Actions::Might<fsm::Actions::TransitionTo<InGameState>> ConnectingToServerState::HandleEvent( const ServerMessageEvent& e )
	{
		using namespace Networking::ClientServer;

		switch (e.message.GetType())
		{
			case MessageFactory::GetMessageType<Messages::ServerClientLoginSuccess>() :
			{
				const auto& result = static_cast<const Messages::ServerClientLoginSuccess&>(e.message);
				LOG_INFO( Client, "Login success message received, our username is '{}'", result.username.data() );
				e.FlagAsHandled();
				return fsm::Actions::TransitionTo<ClientStates::InGameState>();
				break;
			}

			default:
				break;
		}

		return fsm::Actions::NoAction{};
	}

	fsm::Actions::NoAction ConnectingToServerState::HandleEvent( const ConnectedToServerEvent& e )
	{
		using namespace Networking::ClientServer;

		AttachToConnection( e.connection );

		e.connection.SendMessage<Messages::ClientServerLoginStart>( ChannelType::Reliable, []( Messages::ClientServerLoginStart& msg )
			{
				StringUtility::StringToArray( "PLACEHOLDER", msg.username );
			} );

		return fsm::Actions::NoAction();
	}

	fsm::Actions::TransitionTo<DisconnectedFromServerState> ConnectingToServerState::HandleEvent( const DisconnectedFromServerEvent& e )
	{
		DetatchFromConnection( e.connection );
		return fsm::Actions::TransitionTo<DisconnectedFromServerState>();
	}

	void ConnectingToServerState::AttachToConnection( Networking::ClientServer::ServerConnection& connection_ )
	{
		assert( connection == nullptr );
		connection = &connection_;
	}

	void ConnectingToServerState::DetatchFromConnection( Networking::ClientServer::ServerConnection& connection_ )
	{
		assert( connection != nullptr );
		assert( &connection_ == connection );
		connection = nullptr;
	}
}
