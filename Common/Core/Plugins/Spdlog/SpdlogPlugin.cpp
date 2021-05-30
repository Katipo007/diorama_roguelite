#include "SpdlogPlugin.hpp"

#include <cstdio>
#include <cstdarg>
#include <memory>
#include <unordered_map>

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

using namespace API;

namespace
{
	constexpr std::string_view DefaultFileOutputPattern = "[%T][%n][%l] %v";
	const auto VoidLogger = std::make_shared<spdlog::logger>( "VOID" );

	std::unordered_map<LoggingAPI::ChannelId, std::shared_ptr<spdlog::logger>> static_loggers;

	spdlog::level::level_enum TranslateLogLevel( LoggingAPI::Level in )
	{
		switch (in)
		{
		case LoggingAPI::Level::Critical:	return spdlog::level::critical;
		case LoggingAPI::Level::Error:		return spdlog::level::err;
		case LoggingAPI::Level::Warning:	return spdlog::level::warn;
		case LoggingAPI::Level::Info:		return spdlog::level::info;
		case LoggingAPI::Level::Debug:		return spdlog::level::debug;
		case LoggingAPI::Level::Trace:		return spdlog::level::trace;

		default:							return spdlog::level::n_levels;
		}
	}
}

namespace Plugins
{
	SpdlogPlugin::SpdlogPlugin( Filepath log_file_directory )
		: LoggingAPI( log_file_directory )
	{
	}

	SpdlogPlugin::~SpdlogPlugin()
	{
	}

	void SpdlogPlugin::Init()
	{
	}

	void SpdlogPlugin::Shutdown()
	{
	}

	void SpdlogPlugin::AddSinkInternal( ChannelId channel, const Sink& definition )
	{
		assert( !static_loggers.contains( channel ) );

		std::vector<spdlog::sink_ptr> sinks;

		// file output
		if (definition.output_filename.has_value())
		{
			assert( !definition.output_filename.value().empty() );
			auto& sink = sinks.emplace_back( std::make_shared<spdlog::sinks::basic_file_sink_mt>( ( log_file_directory / definition.output_filename.value() ).string(), true ) );

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
		auto [it, success] = static_loggers.emplace( channel, std::make_shared<spdlog::logger>( definition.name, std::begin( sinks ), std::end( sinks ) ) );
		assert( success );
		it->second->set_level( TranslateLogLevel( definition.level ) );
		it->second->flush_on( TranslateLogLevel( definition.flush_on ) );

		spdlog::register_logger( it->second );
	}

	void SpdlogPlugin::LogInternal( ChannelId channel, Level level, std::string_view msg )
	{
		if (const auto found_it = static_loggers.find( channel ); found_it != std::end( static_loggers ))
			found_it->second->log( TranslateLogLevel( level ), msg );
		else
			assert( false && "No logger initialised for channel" );
	}
}
