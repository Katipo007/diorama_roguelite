#pragma once

#include "Common/Utility/Signal.hpp"

namespace Game::Networking { class ClientServerSession; }

namespace UI
{
	class ChatWindow
	{
		static const size_t InputBufferLen = 128;

	public:
		ChatWindow();
		ChatWindow( const ChatWindow& to_copy ) = default;
		ChatWindow( ChatWindow&& to_move ) = default;
		virtual ~ChatWindow();

		// returns true if we just sent a message
		bool Do();

		void AddMessage( std::string_view msg );
		void Clear();

		void SetClientServerSession( Game::Networking::ClientServerSession* const session );

	protected:
		bool SendMessage( std::string_view msg );

	protected:
		char input_buffer[InputBufferLen];

		bool auto_scroll = true;
		bool scroll_to_bottom = false;
		std::vector<std::string> messages;

		Game::Networking::ClientServerSession* client_server_session = nullptr;
	};
}
