#include "ChatWindow.hpp"

#include "Visual/DearImGui/DearImGui.hpp"

namespace
{
	// Portable helpers
	//static int   Stricmp( const char* s1, const char* s2 ) { int d; while ((d = toupper( *s2 ) - toupper( *s1 )) == 0 && *s1) { s1++; s2++; } return d; }
	//static int   Strnicmp( const char* s1, const char* s2, int n ) { int d = 0; while (n > 0 && (d = toupper( *s2 ) - toupper( *s1 )) == 0 && *s1) { s1++; s2++; n--; } return d; }
	//static char* Strdup( const char* s ) { size_t len = strlen( s ) + 1; void* buf = malloc( len ); IM_ASSERT( buf ); return (char*)memcpy( buf, (const void*)s, len ); }
	static void  Strtrim( char* s ) { char* str_end = s + strlen( s ); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }
}

namespace UI
{
	ChatWindow::ChatWindow()
	{
		Clear();
		memset( input_buffer, 0, sizeof( input_buffer ) );
	}

	ChatWindow::~ChatWindow()
	{
		Clear();
	}

	bool ChatWindow::Do()
	{
		bool sent = false;

		ImGui::SetNextWindowSize( ImVec2( 200, 140 ), ImGuiCond_FirstUseEver );
		if (ImGui::Begin( "Chat", NULL, ImGuiWindowFlags_NoCollapse ))
		{
			// Reserve enough left-over height for 1 separator + 1 input text
			{
				const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
				ImGui::BeginChild( "ScrollingRegion", ImVec2( 0, -footer_height_to_reserve ), false, ImGuiWindowFlags_HorizontalScrollbar );

				// Provide option to clear the message history
				if (ImGui::BeginPopupContextWindow())
				{
					ImGui::Checkbox( "Auto-scroll", &auto_scroll );

					if (ImGui::Button( "Clear" ))
					{
						Clear();
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}

				// List messages
				ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 4, 1 ) ); // Tighten spacing

				for (const auto& message : messages)
				{
					// TODO: colours
					ImGui::TextUnformatted( message.c_str() );
				}

				if (scroll_to_bottom || (auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
					ImGui::SetScrollHereY( 1.f );
				scroll_to_bottom = false;

				ImGui::PopStyleVar();
				ImGui::EndChild();
				ImGui::Separator();
			}

			// Entry line
			{
				bool reclaim_focus = false;
				ImGuiInputTextFlags input_text_flags =
					ImGuiInputTextFlags_EnterReturnsTrue
					//| ImGuiInputTextFlags_CallbackCompletion
					//| ImGuiInputTextFlags_CallbackHistory
					;

				const auto input_callback = []( ImGuiInputTextCallbackData* data ) -> int
				{
					switch (data->EventFlag)
					{
					case ImGuiInputTextFlags_CallbackCompletion:
						// TODO: auto-completion
						break;

					case ImGuiInputTextFlags_CallbackHistory:
						// TODO: history
						break;

					default:
						// nothing
						break;
					}

					return 0;
				};

				if (ImGui::InputText( "Input", input_buffer, IM_ARRAYSIZE( input_buffer ), input_text_flags, input_callback ))
				{
					char* s = input_buffer;
					Strtrim( s );
					if (SendMessage( s ))
					{
						sent = true;
						strncpy_s( input_buffer, "", InputBufferLen );
					}
					reclaim_focus = true;
				}

				// Auto-focus on the window apparition
				ImGui::SetItemDefaultFocus();
				if (reclaim_focus)
					ImGui::SetKeyboardFocusHere( -1 ); // auto-focus previos widget
			}

			ImGui::End();
		}

		return sent;
	}

	void ChatWindow::AddMessage( std::string_view msg )
	{
		messages.emplace_back( msg );
	}

	void ChatWindow::Clear()
	{
		messages.clear();
	}

	void ChatWindow::SetClientServerSession( Game::Networking::ClientServerSession* const session )
	{
		if (client_server_session != nullptr)
		{
		}

		client_server_session = session;

		if (client_server_session != nullptr)
		{
		}
	}

	bool ChatWindow::SendMessage( std::string_view msg )
	{
		if (!client_server_session || msg.length() < 3)
			return false;

		NOT_IMPLEMENTED;

		return false;
	}
}
