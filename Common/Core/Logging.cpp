#include "Logging.hpp"


#pragma warning(push, 0) // This ignores all warnings raised inside External headers
#define SPDLOG_COMPILED_LIB
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#pragma warning(pop)

#ifdef _DEBUG
#	pragma comment(lib, "Common/Vendor/spdlog/build/Debug/spdlogd.lib")
#else
#	pragma comment(lib, "Common/Vendor/spdlog/build/Release/spdlog.lib")
#endif

namespace
{
	constexpr std::string_view DefaultFileOutputPattern = "[%T][%n][%l] %v";
	const auto VoidLogger = std::make_shared<spdlog::logger>( "VOID" );
}

Logger::Logger( Filepath log_file_directory )
	: log_file_directory( log_file_directory )
{
}

void Logger::Initialise( Filepath log_file_directory )
{
	assert( static_instance == nullptr );
	if (static_instance != nullptr)
		return;

	static_instance.reset( new Logger( log_file_directory ) );
}

Logger::~Logger()
{
}

void Logger::AddSink( LoggerChannelId channel, const Sink& definition )
{
	assert( !loggers.contains( channel ) );

	std::vector<spdlog::sink_ptr> sinks;

	// file output
	if (definition.output_filename.has_value())
	{
		assert( !definition.output_filename.value().empty() );
		auto& sink = sinks.emplace_back( std::make_shared<spdlog::sinks::basic_file_sink_mt>( (log_file_directory / definition.output_filename.value()).string(), true ) );

		if (definition.file_output_pattern.has_value())
			sink->set_pattern( definition.file_output_pattern.value() );
		else
			sink->set_pattern( std::string{ DefaultFileOutputPattern } );
	}

	// window output
	if (definition.window_output_pattern.has_value())
	{
		auto& sink = sinks.emplace_back( std::make_shared<spdlog::sinks::stderr_color_sink_mt>() );
		sink->set_pattern( definition.window_output_pattern.value() );
	}

	// create the logger
	auto [it, success] = loggers.emplace( channel, std::make_shared<spdlog::logger>( definition.name, std::begin( sinks ), std::end( sinks ) ) );
	assert( success );
	it->second->set_level( TranslateLogLevel( definition.level ) );
	it->second->flush_on( TranslateLogLevel( definition.flush_on ) );

	spdlog::register_logger( it->second );
}

spdlog::level::level_enum Logger::TranslateLogLevel( Level in )
{
	switch (in)
	{
	case Level::Critical:	return spdlog::level::critical;
	case Level::Error:		return spdlog::level::err;
	case Level::Warning:	return spdlog::level::warn;
	case Level::Info:		return spdlog::level::info;
	case Level::Debug:		return spdlog::level::debug;
	case Level::Trace:		return spdlog::level::trace;

	default:				return spdlog::level::n_levels;
	}
}
