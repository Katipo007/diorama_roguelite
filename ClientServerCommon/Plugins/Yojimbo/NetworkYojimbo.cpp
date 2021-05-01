#include "NetworkYojimbo.hpp"

#include "Common/Networking/ChannelTypesEnum.hpp"
#include "Common/Networking/Message.hpp"
#include "Common/Networking/MessageFactory.hpp"

#include "YojimboHeader.hpp"
#include "ClientYojimbo.hpp"
#include "YojimboNetworkAdapters.hpp"

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
}

namespace Plugins
{
	using namespace ::Plugins::Network::Yojimbo;

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

	std::unique_ptr<::Networking::Client> NetworkYojimbo::CreateClient( ::Networking::ClientProperties&& properties )
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

		std::shared_ptr<::Networking::IMessageFactory> message_factory;
		message_factory.reset( properties.message_factory.release() );

		std::unique_ptr<ClientYojimbo> client;
		try
		{
			client.reset( new ClientYojimbo( std::move( target ), properties.private_key, std::move( config ), ClientAdapter( message_factory ) ) );
		}
		catch ( std::runtime_error& e )
		{
			LOG_CRITICAL( Application, "Exception trying to create yojimbo client: {}", e.what() );
		}

		return client;
	}

	std::unique_ptr<::Networking::Server> NetworkYojimbo::CreateServer( ::Networking::ServerProperties&& properties )
	{
		(void)properties;
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
