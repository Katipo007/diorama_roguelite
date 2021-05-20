#pragma once

#include <memory>

#include "../impl/YojimboHeader.hpp"

#include "Common/Utility/Signal.hpp"

namespace YojimboPlugin
{	
	class MessageFactory;

	class BasicAdapter final
		: public yojimbo::Adapter
	{
		using MessageFactoryPtr = std::shared_ptr<MessageFactory>;

	public:
		BasicAdapter( MessageFactoryPtr message_factory );

		MessageFactory& GetFactory() const { return *message_factory; }

		sigslot::signal<BasicAdapter&, int> ServerClientConnected; // Args: adapter, client index
		sigslot::signal<BasicAdapter&, int> ServerClientDisconnected; // Args: adapter, client index

	private:
		yojimbo::MessageFactory* CreateMessageFactory( yojimbo::Allocator& allocator ) override;

		void OnServerClientConnected( int client_index ) override;
		void OnServerClientDisconnected( int client_index ) override;

	protected:
		MessageFactoryPtr message_factory;
	};
}
