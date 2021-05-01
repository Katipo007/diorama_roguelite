#pragma once

#include <memory>

#include "YojimboHeader.hpp"

#include "Common/Utility/Signal.hpp"

namespace Networking { class IMessageFactory; }

namespace Plugins::Network::Yojimbo
{
	///
	/// MessageFactoryWrapper
	/// 
	
	class MessageFactoryWrapper final
		: public yojimbo::MessageFactory
	{
	public:
		MessageFactoryWrapper( yojimbo::Allocator& allocator, std::shared_ptr<::Networking::IMessageFactory> factory_impl );

	protected:
		yojimbo::Message* CreateMessageInternal( int type_idx ) override;

	private:
		std::shared_ptr<::Networking::IMessageFactory> factory_impl;
	};


	/// 
	/// ClientAdapter
	/// 
	
	class ClientAdapter final
		: public yojimbo::Adapter
	{
	public:
		ClientAdapter( std::shared_ptr<::Networking::IMessageFactory> message_factory );

		::Networking::IMessageFactory& GetFactory() const { return *message_factory; }

	private:
		yojimbo::MessageFactory* CreateMessageFactory( yojimbo::Allocator& allocator ) override;

	protected:
		std::shared_ptr<::Networking::IMessageFactory> message_factory;
	};


	///
	/// ServerAdapter
	/// 
	
	class ServerAdapter final
		: public yojimbo::Adapter
	{
	public:
		ServerAdapter( std::shared_ptr<::Networking::IMessageFactory> message_factory );

		::Networking::IMessageFactory& GetFactory() const { return *message_factory; }

		sigslot::signal<ServerAdapter&, int> ClientConnectionEstablished; // Args: adapter, client index
		sigslot::signal<ServerAdapter&, int> ClientConnectionLost; // Args: adapter, client index

	private:
		yojimbo::MessageFactory* CreateMessageFactory( yojimbo::Allocator& allocator ) override;
		void OnServerClientConnected( int client_index ) override;
		void OnServerClientDisconnected( int client_index ) override;

	protected:
		std::shared_ptr<::Networking::IMessageFactory> message_factory;
	};
}
