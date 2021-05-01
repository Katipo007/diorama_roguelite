#include "YojimboNetworkAdapters.hpp"

#include "Common/Networking/Message.hpp"
#include "Common/Networking/MessageFactory.hpp"

namespace Plugins::Network::Yojimbo
{
	///
	/// MessageFactoryWrapper
	/// 
	
	MessageFactoryWrapper::MessageFactoryWrapper( yojimbo::Allocator& allocator, std::shared_ptr<::Networking::IMessageFactory> factory_impl_ )
		: MessageFactory( allocator, static_cast<int>(factory_impl_->GetNumMessageTypes()) )
		, factory_impl( factory_impl_ )
	{
		if (!factory_impl)
			throw std::invalid_argument("Message factory must not be null");
	}

	yojimbo::Message* MessageFactoryWrapper::CreateMessageInternal( int type_idx )
	{
		ASSERT( factory_impl );

		// Get information about the message type
		const size_t message_size = factory_impl->GetSizeOfMessageType( static_cast<::Networking::MessageType>(type_idx) );
		ASSERT( message_size > 0 );
		if (message_size <= 0)
			return NULL;

		// Allocate the yojimbo memory
		yojimbo::Allocator& allocator = GetAllocator();
		auto* yojimbo_message = static_cast<yojimbo::Message*>(allocator.Allocate( message_size, __FILE__, __LINE__ ));
		if (!yojimbo_message)
			return NULL;
		SetMessageType( yojimbo_message, type_idx );

		return yojimbo_message;
	}


	/// 
	/// ClientAdapter
	/// 
	
	ClientAdapter::ClientAdapter( std::shared_ptr<::Networking::IMessageFactory> message_factory_ )
		: yojimbo::Adapter()
		, message_factory( message_factory_ )
	{
		if (!message_factory)
			throw std::invalid_argument( "Message factory must not be null" );
	}

	yojimbo::MessageFactory* ClientAdapter::CreateMessageFactory( yojimbo::Allocator& allocator )
	{
		return YOJIMBO_NEW( allocator, MessageFactoryWrapper, allocator, message_factory );
	}


	/// 
	/// ServerAdapter
	/// 

	ServerAdapter::ServerAdapter( std::shared_ptr<::Networking::IMessageFactory> message_factory_ )
		: yojimbo::Adapter()
		, message_factory( message_factory_ )
	{
	}

	yojimbo::MessageFactory* ServerAdapter::CreateMessageFactory( yojimbo::Allocator& allocator )
	{
		return YOJIMBO_NEW( allocator, MessageFactoryWrapper, allocator, message_factory );
	}

	void ServerAdapter::OnServerClientConnected( int client_index )
	{
		ClientConnectionEstablished( *this, client_index );
	}

	void ServerAdapter::OnServerClientDisconnected( int client_index )
	{
		ClientConnectionLost( *this, client_index );
	}
}
