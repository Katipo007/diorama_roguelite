#pragma once

#include <array>
#include <string_view>

namespace StringUtility
{
	template<size_t ARRAY_SIZE>
	size_t StringToArray( std::string_view str_, std::array<char, ARRAY_SIZE>& array_ )
	{
		static_assert(ARRAY_SIZE > 0);
		const auto src_len = strlen( str_.data() );
		ASSERT( src_len < ARRAY_SIZE );

		memcpy_s( array_.data(), ARRAY_SIZE, str_.data(), src_len );
		array_.back() = 0;
		return std::min( ARRAY_SIZE, src_len );
	}
}
