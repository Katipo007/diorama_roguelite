#include "YojimboPlugin.hpp"

#include "Message.hpp"
#include "MessageFactory.hpp"

#include "Impl/YojimboHeader.hpp"
#include "Client.hpp"
#include "Server.hpp"

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
}

namespace Plugins
{
	YojimboPlugin::YojimboPlugin()
	{
	}

	YojimboPlugin::~YojimboPlugin()
	{
	}

	void YojimboPlugin::Init()
	{
		if (!InitializeYojimbo())
			throw std::runtime_error( "Failed to initialize Yojimbo library" );

#ifdef _DEBUG
		yojimbo_log_level( YOJIMBO_LOG_LEVEL_INFO );
#endif
		yojimbo_set_printf_function( YojimboLoggingRoute );
	}

	void YojimboPlugin::Shutdown()
	{
		ShutdownYojimbo();
	}

	void YojimboPlugin::Update( const PreciseTimestep& ts )
	{
		(void)ts;
	}
}
