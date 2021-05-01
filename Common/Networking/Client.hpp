#pragma once

#include "Common/Utility/Timestep.hpp"
#include "Common/Utility/Signal.hpp"

#include "Address.hpp"
#include "Types.hpp"

namespace API { class NetworkAPI; }

namespace Networking
{
	class Message;
	using ClientMessageHandlerFunc_T = std::function<void( const Message& )>;
	class IMessageFactory;

	struct ClientProperties
	{
		Address target_address;
		Key_T private_key;
		std::unique_ptr<IMessageFactory> message_factory;
		ClientMessageHandlerFunc_T message_handler_func;
	};

	class Client
	{
		friend class API::NetworkAPI;

	public:
		enum class ConnectionState { Disconnected, Connecting, Connected };

	public:
		virtual ~Client() {}

		virtual ConnectionState GetState() const noexcept = 0;

		virtual void Disconnect() = 0;

		sigslot::signal<Client&> ConnectionStateChanged;

	protected:
		virtual void Update( const PreciseTimestep& timestep ) = 0;

		ClientMessageHandlerFunc_T message_handler_func;
	};
}
