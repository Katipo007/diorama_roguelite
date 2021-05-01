#include "NetworkYojimbo.hpp"
#include "YojimboHeader.hpp"

#include "Common/Networking/ChannelTypesEnum.hpp"
#include "Common/Networking/Message.hpp"
#include "Common/Networking/MessageFactory.hpp"
#include "ClientYojimbo.hpp"

namespace
{
	int YojimboLoggingRoute( const char* fmt, ... )
	{
		char buffer[4 * 1024];
		va_list args;
		va_start( args, fmt );
		vsprintf_s( buffer, fmt, args );
		va_end( args );
		const size_t length = strlen( buffer );
		if (buffer[length - 1] == '\n')
			buffer[length - 1] = '\0';

		LOG_INFO( Client, "[yojimbo] {}", buffer );
		return 0;
	}

	void InitDefaultReliableChannel( yojimbo::ChannelConfig& channel )
	{
		channel.type = yojimbo::ChannelType::CHANNEL_TYPE_RELIABLE_ORDERED;
	}

	void InitDefaultUnreliableChannel( yojimbo::ChannelConfig& channel )
	{
		channel.type = yojimbo::ChannelType::CHANNEL_TYPE_UNRELIABLE_UNORDERED;
	}
	
	class MessageFactoryWrapper final
		: public yojimbo::MessageFactory
	{
	public:
		MessageFactoryWrapper( yojimbo::Allocator& allocator, std::unique_ptr<::Networking::IMessageFactory> factory_impl_ )
			: MessageFactory( allocator, factory_impl_->GetNumMessageTypes() )
			, factory_impl( std::move( factory_impl_ ) )
		{
			ASSERT( factory_impl );
		}

		yojimbo::Message* CreateMessageInternal( int type_idx )
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

	private:
		std::unique_ptr<::Networking::IMessageFactory> factory_impl;
	};
}

namespace API
{
	NetworkYojimbo::NetworkYojimbo()
	{
	}

	NetworkYojimbo::~NetworkYojimbo()
	{
	}

	std::string_view NetworkYojimbo::GetName() const noexcept
	{
		return "Yojimbo";
	}

	std::unique_ptr<::Networking::Client> NetworkYojimbo::CreateClient( const::Networking::ClientProperties& properties )
	{
		if (!properties.message_factory)
			throw std::runtime_error( "No message factory specified" );

		if (!properties.message_handler_func)
			throw std::runtime_error( "No message handler specified" );

		yojimbo::Address target( properties.target_address.value.c_str(), static_cast<uint16_t>(properties.target_address.port) );
		if (!target.IsValid())
			throw std::runtime_error( "Invalid connection address" );

		// Setup config
		yojimbo::ClientServerConfig config;
		{
			static_assert(static_cast<size_t>(::Networking::ChannelType::NumChannelTypes) == 2, "Needs updating");
			config.networkSimulator = false;

			config.numChannels = 2;
			InitDefaultReliableChannel( config.channel[static_cast<size_t>(::Networking::ChannelType::Reliable)] );
			InitDefaultUnreliableChannel( config.channel[static_cast<size_t>(::Networking::ChannelType::Unreliable)] );
		}

		// Setup adaptor
		class ClientAdapter
			: public yojimbo::Adapter
		{
		public:
			ClientAdapter( std::shared_ptr<> )

			yojimbo::MessageFactory* CreateMessageFactory( yojimbo::Allocator& allocator ) override
			{
				return YOJIMBO_NEW( allocator, MessageFactoryWrapper, allocator, std::move( properties.message_factory ) );
			}

		};

		std::unique_ptr<ClientYojimbo> client;
		try
		{
			client.reset( new ClientYojimbo( std::move( target ), properties.private_key, std::move( config ), std::move( adapter ) ) );
		}
		catch ( std::runtime_error& e )
		{
			LOG_CRITICAL( Application, "Exception trying to create yojimbo client: {}", e.what() );
		}

		return client;
	}

	std::unique_ptr<::Networking::Server> NetworkYojimbo::CreateServer( const::Networking::ServerProperties& properties )
	{
		NOT_IMPLEMENTED;
		return std::unique_ptr<::Networking::Server>();
	}

	void NetworkYojimbo::Init()
	{
		if (!InitializeYojimbo())
			throw std::runtime_error( "Failed to initialize Yojimbo library" );

#ifdef _DEBUG
		yojimbo_log_level( YOJIMBO_LOG_LEVEL_INFO );
#endif
		yojimbo_set_printf_function( YojimboLoggingRoute );
	}

	void NetworkYojimbo::Shutdown()
	{
		ShutdownYojimbo();
	}

	void NetworkYojimbo::Update( const PreciseTimestep& ts )
	{
		(void)ts;
	}
}
