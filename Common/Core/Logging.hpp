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
	enum class Type
	{
		Client,
		Server,

		NumTypes
	};

	class Log
	{
	public:
		static void Init( const Type type );

		static std::shared_ptr<spdlog::logger>& GetClientLogger();
		static std::shared_ptr<spdlog::logger>& GetServerLogger();
	};
}

#define CLOG_TRACE(...)			::Logging::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CLOG_INFO(...)			::Logging::Log::GetClientLogger()->info(__VA_ARGS__)
#define CLOG_WARN(...)			::Logging::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CLOG_ERROR(...)			::Logging::Log::GetClientLogger()->error(__VA_ARGS__)
#define CLOG_CRITICAL(...)		::Logging::Log::GetClientLogger()->critical(__VA_ARGS__)

#define SLOG_TRACE(...)			::Logging::Log::GetServerLogger()->trace(__VA_ARGS__)
#define SLOG_INFO(...)			::Logging::Log::GetServerLogger()->info(__VA_ARGS__)
#define SLOG_WARN(...)			::Logging::Log::GetServerLogger()->warn(__VA_ARGS__)
#define SLOG_ERROR(...)			::Logging::Log::GetServerLogger()->error(__VA_ARGS__)
#define SLOG_CRITICAL(...)		::Logging::Log::GetServerLogger()->critical(__VA_ARGS__)

/// TODO: Swap default logger between server and client somehow

#define LOG_TRACE(...)			CLOG_TRACE(__VA_ARGS__)
#define LOG_INFO(...)			CLOG_INFO(__VA_ARGS__)
#define LOG_WARN(...)			CLOG_WARN(__VA_ARGS__)
#define LOG_ERROR(...)			CLOG_ERROR(__VA_ARGS__)
#define LOG_CRITICAL(...)		CLOG_CRITICAL(__VA_ARGS__)
