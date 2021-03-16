#pragma once

#include <filesystem>

#include "Base.hpp"
#include "Logging.hpp"

#if defined(ENABLE_ASSERTS)

#	define INTERNAL_ASSERT_IMPL(channel, check, msg, ...) { if(!(check)) { SPDLOG_LOGGER_ERROR( ::Logging::GetLogger( Logging::Channel::channel ), msg, __VA_ARGS__ ); DEBUGBREAK(); } }
#	define INTERNAL_ASSERT_WITH_MSG(channel, check, ...) INTERNAL_ASSERT_IMPL(channel, check, "Assertion failed: {2} at {0}:{1}", std::filesystem::path(__FILE__).filename().string(), __LINE__, __VA_ARGS__)
#	define INTERNAL_ASSERT_NO_MSG(channel, check) INTERNAL_ASSERT_IMPL(channel, check, "Assertion '{0}' failed at {1}:{2}", STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#	define INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#	define INTERNAL_ASSERT_GET_MACRO(...) EXPAND_MACRO( INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, INTERNAL_ASSERT_WITH_MSG, INTERNAL_ASSERT_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#	define ASSERT(...) EXPAND_MACRO( INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(Assert, __VA_ARGS__) )
#	define ASSERT_CHANNEL(channel, ...) EXPAND_MACRO( INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(channel, __VA_ARGS__) )

#	define NOT_IMPLEMENTED { LOG_ERROR( Assert, "Function not implemented!" ); DEBUGBREAK(); }
#	define UNREACHABLE { LOG_ERROR( Assert, "This line should be unreachable!" ); DEBUGBREAK(); }
#else
#	define ASSERT(x, ...) (void)0
#	define ASSERT_CHANNEL(channel, x, ...) (void)0
#	define NOT_IMPLEMENTED (void)0

#	define NOT_IMPLEMENTED { LOG_ERROR( Assert, "Function not implemented!" ); }
#	define UNREACHABLE { LOG_ERROR( Assert, "This line should be unreachable!" ); }
#endif

#define FATAL(msg) { LOG_CRITICAL( Assert, "Application has encountered a fatal error and will abort. {0}", msg ); DEBUGBREAK(); abort(); }
