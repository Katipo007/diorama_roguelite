#pragma once

#include "Common/Utility/Signal.hpp"

namespace UI
{
	class ChatWindow
	{
		static const size_t InputBufferLen = 128;

	public:
		ChatWindow();
		virtual ~ChatWindow();

		// returns true if we just sent a message
		bool Do();

		void AddMessage( std::string_view msg );
		void Clear();

		sigslot::signal<std::string_view> EnteredMessage;

	protected:
		char input_buffer[InputBufferLen];

		bool auto_scroll = true;
		bool scroll_to_bottom = false;
		std::vector<std::string> messages;
	};
}
