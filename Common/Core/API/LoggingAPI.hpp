#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <cstdarg>

#include "CoreAPIsEnum.hpp"
#include "BaseAPI.hpp"
#include "Common/File/Filepath.hpp"
#include "Common/Utility/Hashing.hpp"
#include "Common/Vendor/spdlog/include/spdlog/fmt/fmt.h"

namespace API
{
	class LoggingAPI
		: public BaseAPI
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

		struct ChannelId
		{
			inline constexpr ChannelId( std::string_view id ) : value( CompileTime::FNV16Hash( id.data() ) ) {}
			inline constexpr ChannelId( uint16_t id ) : value( id ) {}

			uint16_t value;

			constexpr bool operator==( const ChannelId& other ) const { return value == other.value; }
		};

	public:
		virtual ~LoggingAPI();

		static constexpr APIType GetType() noexcept { return CoreAPIs::Logging; }

		void AddSink( ChannelId channel, const Sink& sink );
		template<typename ...Args>
		static void Log( ChannelId channel, Level level, std::string_view fmt, Args&&... args ) { GetInstance().LogInternal( channel, level, fmt::format( fmt, std::forward<Args>( args )... ) ); }

		static bool IsInitialised() noexcept { return static_instance != nullptr; }
		static LoggingAPI& GetInstance() noexcept { return *static_instance; }

	protected:
		explicit LoggingAPI( Filepath log_file_directory );

		virtual void AddSinkInternal( ChannelId channel, const Sink& sink ) = 0;
		virtual void LogInternal( ChannelId channel, Level level, std::string_view msg ) = 0;

	protected:
		const Filepath log_file_directory;

	private:
		inline static LoggingAPI* static_instance = nullptr;
	};
}

namespace LoggingChannels
{
	constexpr ::API::LoggingAPI::ChannelId Assertion( "Assertion" );
	constexpr ::API::LoggingAPI::ChannelId Application( "Application" );
}

namespace std
{
	template <>
	struct hash<API::LoggingAPI::ChannelId>
	{
		std::size_t operator()( const API::LoggingAPI::ChannelId& k ) const { return k.value; }
	};
}

#define LOGGING_API_CALL(channel, level, fmt, ...) ::API::LoggingAPI::Log( channel, level, fmt, __VA_ARGS__ )
#define LOG_TRACE(channel, fmt, ...)		LOGGING_API_CALL( channel, ::API::LoggingAPI::Level::Trace, fmt, __VA_ARGS__ )
#define LOG_INFO(channel, fmt, ...)			LOGGING_API_CALL( channel, ::API::LoggingAPI::Level::Info, fmt, __VA_ARGS__ )
#define LOG_WARN(channel, fmt, ...)			LOGGING_API_CALL( channel, ::API::LoggingAPI::Level::Warning, fmt, __VA_ARGS__ )
#define LOG_ERROR(channel, fmt, ...)		LOGGING_API_CALL( channel, ::API::LoggingAPI::Level::Error, fmt, __VA_ARGS__ )
#define LOG_CRITICAL(channel, fmt, ...)		LOGGING_API_CALL( channel, ::API::LoggingAPI::Level::Critical, fmt, __VA_ARGS__ )
