#include "Precompiled/pch.hpp"
#include "Logging.hpp"

#ifdef _DEBUG
#	pragma comment(lib, "Common/Vendor/spdlog/build/Debug/spdlogd.lib")
#else
#	pragma comment(lib, "Common/Vendor/spdlog/build/Release/spdlog.lib")
#endif

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Logging
{
	namespace
	{
		std::shared_ptr<spdlog::logger> client_logger;
		std::shared_ptr<spdlog::logger> server_logger;
	}

	void Log::Init( const Type type )
	{
		// Setup client logger
		if (type == Type::Client)
		{
			std::vector<spdlog::sink_ptr> log_sinks;
			log_sinks.emplace_back( std::make_shared<spdlog::sinks::stderr_color_sink_mt>() );
			log_sinks.emplace_back( std::make_shared<spdlog::sinks::basic_file_sink_mt>( "Client.log", true ) );

			log_sinks[0]->set_pattern( "%^[%T] %n: %v%$" );
			log_sinks[1]->set_pattern( "[%T] [%l] %n: %v" );

			client_logger = std::make_shared<spdlog::logger>( "CLIENT", begin( log_sinks ), end( log_sinks ) );
			spdlog::register_logger( client_logger );
			client_logger->set_level( spdlog::level::trace );
			client_logger->flush_on( spdlog::level::trace );
		}
		// Setup server logger
		else if (type == Type::Server)
		{
			std::vector<spdlog::sink_ptr> log_sinks;
			log_sinks.emplace_back( std::make_shared<spdlog::sinks::stderr_color_sink_mt>() );
			log_sinks.emplace_back( std::make_shared<spdlog::sinks::basic_file_sink_mt>( "Server.log", true ) );

			log_sinks[0]->set_pattern( "%^[%T] %n: %v%$" );
			log_sinks[1]->set_pattern( "[%T] [%l] %n: %v" );

			server_logger = std::make_shared<spdlog::logger>( "SERVER", begin( log_sinks ), end( log_sinks ) );
			spdlog::register_logger( server_logger );
			server_logger->set_level( spdlog::level::trace );
			server_logger->flush_on( spdlog::level::trace );
		}
		else
			throw std::runtime_error( "Unrecognised logger type" );
	}

	std::shared_ptr<spdlog::logger>& Log::GetClientLogger()
	{
		// purposefully the default assert
		assert( client_logger != nullptr );
		return client_logger;
	}

	std::shared_ptr<spdlog::logger>& Log::GetServerLogger()
	{
		// purposefully the default assert
		assert( server_logger != nullptr );
		return server_logger;
	}
}
