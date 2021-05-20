#include "YojimboPlugin.hpp"

#include "YojimboHeader.hpp"

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

		LOG_INFO( Application, "[yojimbo] {}", buffer );
		return 0;
	}
}

namespace Plugins
{
	bool YojimboPlugin::s_initialised = false;

	YojimboPlugin::YojimboPlugin()
	{
	}

	YojimboPlugin::~YojimboPlugin()
	{
	}

	bool YojimboPlugin::IsInitialised() noexcept
	{
		return YojimboPlugin::s_initialised;
	}

	void YojimboPlugin::Init()
	{
		ASSERT( !IsInitialised() );
		if (IsInitialised())
			return;

		if (!InitializeYojimbo())
			throw std::runtime_error( "Failed to initialize Yojimbo library" );

#ifdef _DEBUG
		yojimbo_log_level( YOJIMBO_LOG_LEVEL_INFO );
#endif
		yojimbo_set_printf_function( YojimboLoggingRoute );

		s_initialised = true;
	}

	void YojimboPlugin::Shutdown()
	{
		ASSERT( IsInitialised() );
		if (!IsInitialised())
			return;

		ShutdownYojimbo();
		s_initialised = false;
	}
}
