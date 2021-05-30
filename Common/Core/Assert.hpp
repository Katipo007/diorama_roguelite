#pragma once

#include <filesystem>

#include "Base.hpp"
#include "Common/Core/API/LoggingAPI.hpp"

#if !defined(ENABLE_ASSERTS)

#	define INTERNAL_ASSERT_IMPL(channel, check, msg, ...) do { if(!(check)) { LOG_ERROR( channel, msg, __VA_ARGS__ ); DEBUGBREAK(); } } while(0)
#	define INTERNAL_ASSERT_WITH_MSG(channel, check, ...) INTERNAL_ASSERT_IMPL(channel, check, "Assertion failed: {2} at {0}:{1}", std::filesystem::path(__FILE__).filename().c_str(), __LINE__, __VA_ARGS__)
#	define INTERNAL_ASSERT_NO_MSG(channel, check) INTERNAL_ASSERT_IMPL(channel, check, "Assertion '{0}' failed at {1}:{2}", STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().c_str(), __LINE__)

#	define INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#	define INTERNAL_ASSERT_GET_MACRO(...) EXPAND_MACRO( INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, INTERNAL_ASSERT_WITH_MSG, INTERNAL_ASSERT_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#	define ASSERT(...) EXPAND_MACRO( INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(::LoggingChannels::Assertion, __VA_ARGS__) )
#	define ASSERT_CHANNEL(channel, ...) EXPAND_MACRO( INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(channel, __VA_ARGS__) )

#	define NOT_IMPLEMENTED do { LOG_ERROR( ::LoggingChannels::Assertion, "Function not implemented!" ); DEBUGBREAK(); } while(0)
#else
#	define ASSERT(x, ...) do {} while(0)
#	define ASSERT_CHANNEL(channel, x, ...) do {} while(0)

#	define NOT_IMPLEMENTED do { LOG_ERROR( ::LoggingChannels::Assertion, "Function not implemented!" ); } while(0)
#endif

#define FATAL(msg) do { LOG_CRITICAL( ::LoggingChannels::Assertion, "Application has encountered a fatal error and will abort. {0}", msg ); DEBUGBREAK(); abort(); } while(0)
