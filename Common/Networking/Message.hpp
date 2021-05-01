#pragma once

namespace Networking
{
	using MessageType = uint32_t;
	class Message
	{
	public:
		virtual ~Message() {}

		//virtual MessageType GetType() const noexcept = 0;

		void SetHandled() noexcept { handled = true; }
		bool IsHandled() const noexcept { return handled; }

	private:
		bool handled = false;
	};
}
