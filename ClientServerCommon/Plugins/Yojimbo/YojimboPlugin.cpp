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
		ASSERT( !initialised );
		if (initialised)
			return;

		if (!InitializeYojimbo())
			throw std::runtime_error( "Failed to initialize Yojimbo library" );

#ifdef _DEBUG
		yojimbo_log_level( YOJIMBO_LOG_LEVEL_INFO );
#endif
		yojimbo_set_printf_function( YojimboLoggingRoute );

		initialised = true;
	}

	void YojimboPlugin::Shutdown()
	{
		ASSERT( initialised );
		if (!initialised)
			return;

		clients.clear();
		servers.clear();
		ShutdownYojimbo();
		initialised = false;
	}

	void YojimboPlugin::OnVariableUpdate( const PreciseTimestep& ts, const StepType type )
	{
		ASSERT( initialised );
		if (!initialised)
			return;

		switch (type)
		{
		case StepType::PreGameStep:
			for (auto& server : servers)
				server->Update( ts );
			for (auto& client : clients)
				client->Update( ts );
			break;

		case StepType::PostGameStep:
			// null
			break;
		}
	}

	void YojimboPlugin::Add( ::YojimboPlugin::Server& server )
	{
		auto it = std::find_if( std::begin( servers ), std::end( servers ), [&server]( const decltype(servers)::value_type& entry ) { return entry == &server; } );
		if (it != std::end( servers ))
			return;

		servers.push_back( &server );
	}

	void YojimboPlugin::Add( ::YojimboPlugin::Client& client )
	{
		auto it = std::find_if( std::begin( clients ), std::end( clients ), [&client]( const decltype(clients)::value_type& entry ) { return entry == &client; } );
		if (it != std::end( clients ))
			return;

		clients.push_back( &client );
	}

	void YojimboPlugin::Remove( ::YojimboPlugin::Server& server )
	{
		auto it = std::find_if( std::begin( servers ), std::end( servers ), [&server]( const decltype(servers)::value_type& entry ) { return entry == &server; } );
		if (it == std::end( servers ))
			return;

		servers.erase( it );
	}

	void YojimboPlugin::Remove( ::YojimboPlugin::Client& client )
	{
		auto it = std::find_if( std::begin( clients ), std::end( clients ), [&client]( const decltype(clients)::value_type& entry ) { return entry == &client; } );
		if (it == std::end( clients ))
			return;

		clients.erase( it );
	}
}
