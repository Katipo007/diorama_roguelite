#pragma once

#include <filesystem>

#include "Base.hpp"
#include "Logging.hpp"

#if defined(ENABLE_ASSERTS)

#	define INTERNAL_ASSERT_IMPL(check, msg, ...) { if(!(check)) { LOG_ERROR(msg, __VA_ARGS__); DEBUGBREAK(); } }
#	define INTERNAL_ASSERT_WITH_MSG(check, ...) INTERNAL_ASSERT_IMPL(check, "Assertion failed: {0}", __VA_ARGS__)
#	define INTERNAL_ASSERT_NO_MSG(check) INTERNAL_ASSERT_IMPL(check, "Assertion '{0}' failed at {1}:{2}", STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#	define INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#	define INTERNAL_ASSERT_GET_MACRO(...) EXPAND_MACRO( INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, INTERNAL_ASSERT_WITH_MSG, INTERNAL_ASSERT_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#	define ASSERT(...) EXPAND_MACRO( INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(__VA_ARGS__) )

#	define NOT_IMPLEMENTED { LOG_ERROR( "Function not implemented!" ); DEBUGBREAK(); }
#	define UNREACHABLE { LOG_ERROR( "This line should be unreachable!" ); DEBUGBREAK(); }
#else
#	define ASSERT(x, ...) (void)0
#	define NOT_IMPLEMENTED (void)0
#endif

#define ENGINE_CORE_CRASH(msg) { LOG_CRITICAL("Application crashed: {0}", msg); DEBUGBREAK(); }
#define ENGINE_CORE_CRASH_F(msg, ...) { LOG_CRITICAL(msg, __VA_ARGS__); DEBUGBREAK(); }