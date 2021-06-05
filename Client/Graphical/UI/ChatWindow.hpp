#pragma once

#include "Common/Utility/NonCopyable.hpp"
#include "Common/Utility/Signal.hpp"

namespace Game::Networking { class ClientServerSession; }

namespace UI
{
	class ChatWindow final
		: NonCopyable
	{
	public:
		ChatWindow();
		virtual ~ChatWindow();

		// returns true if we just sent a message
		bool Do();

		void AddMessage( std::string_view msg );
		void Clear();

		void SetClientServerSession( Game::Networking::ClientServerSession* const session );

	protected:
		bool SendMessage( std::string_view msg );

	protected:
		std::array<char, 128> input_buffer{ 0 };

		bool auto_scroll = true;
		bool scroll_to_bottom = false;
		std::vector<std::string> messages;

		Game::Networking::ClientServerSession* client_server_session = nullptr;
	};
}
