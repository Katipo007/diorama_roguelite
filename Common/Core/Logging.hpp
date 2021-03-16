#pragma once

#include "Common/Core/Base.hpp"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#define SPDLOG_COMPILED_LIB
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Logging
{
	using Logger = spdlog::logger;

	enum class Channel
	{
		//
		// Regular
		//
		Assert,
		Client,
		Server,
		Application,

		//
		// Graphics pipelines
		//
		OpenGL,

		NumChannels
	};

	void InitDefaultClientSinks();
	void InitDefaultServerSinks();

	std::shared_ptr<Logger> CreateStandardLogger( std::string_view name, const bool output_to_window, const spdlog::sink_ptr file_sink = spdlog::sink_ptr() );
	std::shared_ptr<Logger> CreateAdvancedLogger( std::string_view name, std::string_view output_window_pattern, std::string_view file_output_pattern, const spdlog::sink_ptr file_sink = spdlog::sink_ptr() );

	void RegisterLogger( const Channel chnl, std::shared_ptr<Logger> logger );
	const std::shared_ptr<Logger>& GetLogger( const Channel chnl );
}

#define LOG_TRACE(channel, ...)			SPDLOG_LOGGER_TRACE( ::Logging::GetLogger( Logging::Channel::channel ), __VA_ARGS__ )
#define LOG_INFO(channel, ...)			SPDLOG_LOGGER_INFO( ::Logging::GetLogger( Logging::Channel::channel ), __VA_ARGS__ )
#define LOG_WARN(channel, ...)			SPDLOG_LOGGER_WARN( ::Logging::GetLogger( Logging::Channel::channel ), __VA_ARGS__ )
#define LOG_ERROR(channel, ...)			SPDLOG_LOGGER_ERROR( ::Logging::GetLogger( Logging::Channel::channel ), __VA_ARGS__ )
#define LOG_CRITICAL(channel, ...)		SPDLOG_LOGGER_CRITICAL( ::Logging::GetLogger( Logging::Channel::channel ), __VA_ARGS__ )
