#pragma once

#include "Message.hpp"

namespace Networking
{
	class MessageDispatcher
	{
	public:
		MessageDispatcher( ::Networking::Message& message_ )
			: message( message_ )
		{}

		template<typename T, typename F>
		bool Dispatch( const F& func )
		{
			static_assert(std::is_base_of<::Networking::Message, T>::value, "Provided type does not derive from Networking::Message");
			static_assert(std::is_invocable<F, T&>::value, "Provided handler doesn't accept the correct message type");
			static_assert(std::is_same< bool, std::invoke_result<F, T&>::type >::value, "Provided message handler must return a boolean stating whether the message was handled");
			static_assert(std::is_convertible<T::GetId(), MessageId>::value, "Message type is not convertable to internal message id");

			if (message.GetId() == dynamic_cast<::Networking::MessageType>(T::GetMessageId()))
			{
				if (func( dynamic_cast<T&>(message) ))
				{
					this->message.SetHandled();
					return true;
				}
			}
			return false;
		}

	private:
		::Networking::Message& message;
	};
}
