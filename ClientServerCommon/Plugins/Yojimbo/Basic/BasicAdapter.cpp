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
		BasicMessageFactoryWrapper( yojimbo::Allocator& allocator, std::shared_ptr<PluginFactory_T> our_factory_ )
			: LibraryFactory_T( allocator, static_cast<int>( our_factory->GetNumMessageTypes() ) )
			, our_factory( our_factory_ )
		{}
		~BasicMessageFactoryWrapper() = default;

	private:
		yojimbo::Message* CreateMessageInternal( int type ) override
		{
			return our_factory->CreateUntypedMessage( type );
		}

	private:
		std::shared_ptr<PluginFactory_T> our_factory;
	};

	BasicAdapter::BasicAdapter( MessageFactoryPtr message_factory_ )
		: yojimbo::Adapter()
		, message_factory( message_factory_ )
	{
	}

	yojimbo::MessageFactory* BasicAdapter::CreateMessageFactory( yojimbo::Allocator& allocator )
	{
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
