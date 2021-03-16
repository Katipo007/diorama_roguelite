#include "Precompiled/pch.hpp"
#include "Logging.hpp"

#ifdef _DEBUG
#	pragma comment(lib, "Common/Vendor/spdlog/build/Debug/spdlogd.lib")
#else
#	pragma comment(lib, "Common/Vendor/spdlog/build/Release/spdlog.lib")
#endif

#define ONLY_EXECUTE_ONCE { static bool ___only_exec_once___i_ = false; if (___only_exec_once___i_) return; ___only_exec_once___i_ = true; }

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Logging
{
	namespace
	{
		std::string DefaultFileOutputPattern = "[%T][%n][%l] %v";

		spdlog::sink_ptr CreateBasicFileSink( std::string_view filename, std::string_view pattern )
		{
			assert( !filename.empty() );
			assert( !pattern.empty() );

			auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>( std::string( filename ) + ".log", true );
			sink->set_pattern( std::string( pattern ) );
			return sink;
		}

		void InitCommonSinks()
		{
			ONLY_EXECUTE_ONCE;

			RegisterLogger( Channel::Assert, CreateAdvancedLogger( "Asserts", "%^[ASSERT]%$ %v (%@)", "[%T][ASSERT] %v (%@)" ) );
		}

		std::unordered_map<Channel, std::shared_ptr<Logger>> loggers;
	}

	std::shared_ptr<Logger> CreateStandardLogger( std::string_view name, const bool output_to_window, const spdlog::sink_ptr file_sink )
	{
		return CreateAdvancedLogger(
			name,
			output_to_window ? "%^[%n][%l]%$ %v" : "",
			DefaultFileOutputPattern,
			file_sink
		);
	}

	std::shared_ptr<Logger> CreateAdvancedLogger( std::string_view name, std::string_view output_window_pattern, std::string_view file_output_pattern, const spdlog::sink_ptr file_sink )
	{
		std::vector<spdlog::sink_ptr> sinks;
		if (!output_window_pattern.empty())
		{
			sinks.emplace_back( std::make_shared<spdlog::sinks::stderr_color_sink_mt>() );
			sinks.back()->set_pattern( std::string( output_window_pattern ) );
		}

		if (file_sink)
			sinks.emplace_back( file_sink );
		else if( !file_output_pattern.empty() )
			sinks.emplace_back( CreateBasicFileSink( name, file_output_pattern ) );

		auto logger = std::make_shared<spdlog::logger>( std::string( name ), std::begin( sinks ), std::end( sinks ) );
		spdlog::register_logger( logger );

#ifdef _DEBUG
		logger->set_level( spdlog::level::trace );
		logger->flush_on( spdlog::level::trace );
#else
		logger->set_level( spdlog::level::info );
		logger->flush_on( spdlog::level::info );
#endif

		return logger;
	}

	void InitDefaultClientSinks()
	{
		ONLY_EXECUTE_ONCE;

		const auto client_file_sink = CreateBasicFileSink( "Client", DefaultFileOutputPattern );

		InitCommonSinks();
		RegisterLogger( Channel::Application, CreateStandardLogger( "Application", true, client_file_sink ) );
		RegisterLogger( Channel::Client, CreateStandardLogger( "Client", true, client_file_sink ) );

#ifdef RENDERER_IMPLEMENTATION_OPENGL
		RegisterLogger( Channel::OpenGL, CreateStandardLogger( "OpenGL", true, client_file_sink ) );
#endif
	}

	void InitDefaultServerSinks()
	{
		ONLY_EXECUTE_ONCE;

		InitCommonSinks();
		RegisterLogger( Channel::Server, CreateStandardLogger( "Server", true ) );
	}

	void RegisterLogger( const Channel chnl, std::shared_ptr<Logger> logger )
	{
		assert( logger != nullptr );
		if (logger == nullptr)
			return;

		assert( loggers.count( chnl ) == 0 ); // We don't want to replace loggers
		if (loggers.count( chnl ) != 0)
			return;

		loggers[chnl] = logger;
	}

	const std::shared_ptr<Logger>& GetLogger( const Channel chnl )
	{
		static const auto VoidLogger = std::make_shared<Logger>( "VOID" ); // used if a specific channel wasn't initialised

		return (loggers.count( chnl ) > 0) ? loggers[chnl] : VoidLogger;
	}
}
