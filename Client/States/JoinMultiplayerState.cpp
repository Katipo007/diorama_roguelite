#include "JoinMultiplayerState.hpp"

#include "Client/ClientGame.hpp"
#include "Visual/DearImGui/DearImGui.hpp"

namespace ClientStates
{
	JoinMultiplayerState::JoinMultiplayerState()
	{
	}

	JoinMultiplayerState::~JoinMultiplayerState()
	{
	}

	JoinMultiplayerState::ExitActions JoinMultiplayerState::HandleEvent( const FrameEvent& e )
	{
		(void)e;

		if (server_connection)
		{
			server_connection->Update( e.precise_timestep );
			const auto connection_state = server_connection->GetConnectionState();
			switch (connection_state)
			{
			case Sessions::ClientServerSession::ConnectionState::Connected:
				Game::GetClientGame().QueueEvent( ClientStates::ConnectedToServerEvent( std::move( server_connection ) ) );
				status_message = "";
				server_connection.reset();
				break;

			case Sessions::ClientServerSession::ConnectionState::Connecting:
				status_message = "Connecting...";
				break;

			case Sessions::ClientServerSession::ConnectionState::Disconnected:
				status_message = "Connection failed.";
				server_connection.reset();
				break;
			}
		}

		return StateMachine::Actions::NoAction{};
	}

	JoinMultiplayerState::ExitActions JoinMultiplayerState::HandleEvent( const DearImGuiFrameEvent& e )
	{
		(void)e;

		if (ImGui::Begin( "JoinMultiplayerState", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration ))
		{
			ImGui::Text( "Join Multiplayer" );

			const bool is_connecting = server_connection != nullptr;
			if (is_connecting)
				ImGui::PushStyleVar( ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f );

			static char address_textbox_value[128] = "127.0.0.1";
			if (ImGui::InputText( "IP Address", address_textbox_value, 128, ImGuiInputTextFlags_EnterReturnsTrue ) )
			{
				if( !is_connecting )
					InitiateConnection( address_textbox_value );
			}

			if (is_connecting)
			{
				ImGui::PopStyleVar();
				if (ImGui::Button( "Cancel" ))
					CancelConnection();
			}
			else
			{
				if (ImGui::Button( "Connect" ))
					InitiateConnection( address_textbox_value );
			}

			// return to menu
			if (ImGui::Button( "Back" ))
			{
				ImGui::End();
				return StateMachine::Actions::TransitionTo<MainMenuState>{};
			}

			// current status message
			ImGui::TextDisabled( status_message.c_str() );

			ImGui::End();
		}

		return StateMachine::Actions::NoAction{};
	}

	void JoinMultiplayerState::InitiateConnection( std::string address_str )
	{
		ASSERT( server_connection == nullptr );
		if (server_connection != nullptr)
			return;

		auto address = yojimbo::Address( address_str.c_str() );
		server_connection = std::make_unique<Sessions::ClientServerSession>( address );
	}

	void JoinMultiplayerState::CancelConnection()
	{
		if (server_connection)
			server_connection.reset();
	}
}
