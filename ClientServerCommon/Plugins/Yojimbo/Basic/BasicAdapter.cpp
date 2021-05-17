#include "BasicAdapter.hpp"

#include "../Message.hpp"
#include "../MessageFactory.hpp"

using PluginFactory_T = YojimboPlugin::MessageFactory;
using LibraryFactory_T = yojimbo::MessageFactory;

namespace YojimboPlugin
{
	class BasicMessageFactoryWrapper final
		: public LibraryFactory_T
	{
	public:
		BasicMessageFactoryWrapper( yojimbo::Allocator& allocator, std::shared_ptr<PluginFactory_T> factory_ )
			: LibraryFactory_T( allocator, static_cast<int>(factory_->GetNumMessageTypes()) )
			, our_factory( factory_ )
		{
			ASSERT( our_factory );
		}
		~BasicMessageFactoryWrapper() = default;

	private:
		yojimbo::Message* CreateMessageInternal( int type ) override
		{
			ASSERT( our_factory );
			return our_factory ? our_factory->CreateUntypedMessage( type ) : NULL;
		}

	private:
		std::shared_ptr<PluginFactory_T> our_factory;
	};

	BasicAdapter::BasicAdapter( MessageFactoryPtr message_factory_ )
		: yojimbo::Adapter()
		, message_factory( message_factory_ )
	{
		ASSERT( message_factory );
	}

	yojimbo::MessageFactory* BasicAdapter::CreateMessageFactory( yojimbo::Allocator& allocator )
	{
		ASSERT( message_factory );
		return YOJIMBO_NEW( allocator, BasicMessageFactoryWrapper, allocator, message_factory );
	}

	void BasicAdapter::OnServerClientConnected( int client_index )
	{
		// forward to signal
		ServerClientConnected( *this, client_index );
	}

	void BasicAdapter::OnServerClientDisconnected( int client_index )
	{
		// forward to signal
		ServerClientDisconnected( *this, client_index );
	}
}
