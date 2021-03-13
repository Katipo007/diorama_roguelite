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
	class Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetClientLogger();
	};
}

#define LOG_TRACE(...)			::Logging::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)			::Logging::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)			::Logging::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)			::Logging::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)		::Logging::Log::GetClientLogger()->critical(__VA_ARGS__)