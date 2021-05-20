#pragma once

#include <functional>
#include <memory>

#include "Common/Utility/Timestep.hpp"
#include "Common/Utility/Signal.hpp"

#include "Types.hpp"

namespace Plugins { class YojimboPlugin; }

namespace YojimboPlugin
{
	struct Message;

	class BaseClient
	{
		friend class ::Plugins::YojimboPlugin;
		using MessageHandlerFunc_T = std::function<bool( BaseClient&, const Message& )>;

	public:
		enum class ConnectionState { Disconnected, Connecting, Connected };

	public:
		virtual ~BaseClient() {}

		virtual ConnectionState GetState() const noexcept = 0;

		virtual void Disconnect() = 0;

		bool HasMessageHandler() const noexcept { return (bool)message_handler; }
		void SetMessageHandler( MessageHandlerFunc_T handler ) noexcept { message_handler = handler; }

	public: // events
		sigslot::signal<BaseClient&> ConnectionStateChanged;

	protected:
		virtual void Update( const PreciseTimestep& timestep ) = 0;

	protected:
		MessageHandlerFunc_T message_handler;
	};
}
