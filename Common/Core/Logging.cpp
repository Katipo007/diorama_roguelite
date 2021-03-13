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
	}

	void Log::Init()
	{
		std::vector<spdlog::sink_ptr> log_sinks;
		log_sinks.emplace_back( std::make_shared<spdlog::sinks::stderr_color_sink_mt>() );
		log_sinks.emplace_back( std::make_shared<spdlog::sinks::basic_file_sink_mt>( "Client.log", true ) );

		log_sinks[ 0 ]->set_pattern( "%^[%T] %n: %v%$" );
		log_sinks[ 1 ]->set_pattern( "[%T] [%l] %n: %v" );

		client_logger = std::make_shared<spdlog::logger>( "CLIENT", begin( log_sinks ), end( log_sinks ) );
		spdlog::register_logger( client_logger );
		client_logger->set_level( spdlog::level::trace );
		client_logger->flush_on( spdlog::level::trace );
	}

	std::shared_ptr<spdlog::logger>& Log::GetClientLogger()
	{
		// purposefully the default assert
		assert( client_logger != nullptr );
		return client_logger;
	}
}
