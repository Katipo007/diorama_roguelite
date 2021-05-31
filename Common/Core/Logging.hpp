#pragma once

#include <assert.h>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

#include "Common/File/Filepath.hpp"
#include "Common/Utility/Hashing.hpp"

#pragma warning(push, 0) // This ignores all warnings raised inside External headers
#define SPDLOG_COMPILED_LIB
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)


struct LoggerChannelId
{
	inline constexpr LoggerChannelId( std::string_view id ) : value( CompileTime::FNV16Hash( id.data() ) ) {}
	inline constexpr LoggerChannelId( uint16_t id ) : value( id ) {}

	uint16_t value;

	constexpr bool operator==( const LoggerChannelId& other ) const { return value == other.value; }
};

namespace LoggingChannels
{
	constexpr LoggerChannelId Assertion( "Assertion" );
	constexpr LoggerChannelId Application( "Application" );
}

namespace std
{
	template <>
	struct hash<LoggerChannelId>
	{
		std::size_t operator()( const LoggerChannelId& k ) const { return k.value; }
	};
}

class Logger final
{
public:
	enum class Level
	{
		Trace,
		Debug,
		Info,
		Warning,
		Error,
		Critical
	};
#if _DEBUG
	inline static const Level DefaultLevel = Level::Info;
#else
	inline static const Level DefaultLevel = Level::Warning;
#endif

	struct Sink
	{
		std::string name;
		std::optional<Filepath> output_filename;
		std::optional<std::string> window_output_pattern;
		std::optional<std::string> file_output_pattern;
		Level level = DefaultLevel;
		Level flush_on = Level::Warning;
	};

public:
	static void Initialise( Filepath log_file_directory );
	virtual ~Logger();

	void AddSink( LoggerChannelId channel, const Sink& sink );
	template<typename ...Args>
	void Log( LoggerChannelId channel, Level level, std::string_view fmt, Args&&... args )
	{
		if (const auto found_it = loggers.find( channel ); found_it != std::end( loggers ))
			found_it->second->log( spdlog::source_loc{}, TranslateLogLevel( level ), fmt.data(), std::forward<Args>( args )... );
		else
			assert( false && "No logger initialised for channel" );
	}

	inline static bool IsInitialised() noexcept { return static_instance != nullptr; }
	inline static Logger& GetInstance() noexcept { assert( IsInitialised() ); return *static_instance; }

private:
	explicit Logger( Filepath log_file_directory );

	static spdlog::level::level_enum TranslateLogLevel( Level in );

private:
	inline static std::unique_ptr<Logger> static_instance;

	const Filepath log_file_directory;
	std::unordered_map<LoggerChannelId, std::shared_ptr<spdlog::logger>> loggers;
};

#define LOGGING_API_CALL(channel, level, fmt, ...) ::Logger::GetInstance().Log( channel, level, fmt, __VA_ARGS__ )
#define LOG_TRACE(channel, fmt, ...)		LOGGING_API_CALL( channel, ::Logger::Level::Trace, fmt, __VA_ARGS__ )
#define LOG_INFO(channel, fmt, ...)			LOGGING_API_CALL( channel, ::Logger::Level::Info, fmt, __VA_ARGS__ )
#define LOG_WARN(channel, fmt, ...)			LOGGING_API_CALL( channel, ::Logger::Level::Warning, fmt, __VA_ARGS__ )
#define LOG_ERROR(channel, fmt, ...)		LOGGING_API_CALL( channel, ::Logger::Level::Error, fmt, __VA_ARGS__ )
#define LOG_CRITICAL(channel, fmt, ...)		LOGGING_API_CALL( channel, ::Logger::Level::Critical, fmt, __VA_ARGS__ )
