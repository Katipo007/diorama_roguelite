#pragma once

#include <string>
#include <unordered_map>

namespace __detail
{
	struct string_hash
	{
		using is_transparent = void;                    // I confirm I know what I am doing
		using hash_type = std::hash<std::string_view>;  // just a helper local type
		size_t operator()( std::string_view txt ) const { return hash_type{}(txt); }
		size_t operator()( const std::string& txt ) const { return hash_type{}(txt); }
		size_t operator()( const char* txt ) const { return hash_type{}(txt); }
	};
}

/// <summary>
/// std::unordered_map using std::string for the key, uses C++20 features to provide lookups by std::string compatable types
/// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0919r0.html
/// </summary>
/// <typeparam name="Value_T">Value type for the hashmap</typeparam>
template<typename Value_T>
using StringHashMap = std::unordered_map<std::string, Value_T, __detail::string_hash, std::equal_to<>>;
