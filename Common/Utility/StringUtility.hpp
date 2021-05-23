#pragma once

#include <array>
#include <algorithm>
#include <string>
#include <string_view>

namespace StringUtility
{
	template<typename _T, size_t ARRAY_SIZE>
	size_t StringToArray( std::string_view str_, std::array<_T, ARRAY_SIZE>& array_ )
	{
		static_assert(ARRAY_SIZE > 0);

		const size_t str_size = std::distance( std::begin( str_ ), std::end( str_ ) );
		const size_t n_to_copy = std::min( str_size, ARRAY_SIZE );
		if (n_to_copy <= 0)
		{
			array_.fill( static_cast<_T>(0) );
			return 0;
		}
		else
		{
			std::copy_n( std::begin( str_ ), n_to_copy, std::begin( array_ ) );
			std::fill( std::begin( array_ ) + n_to_copy, std::end( array_ ), static_cast<_T>(0) );
			array_.back() = 0;
			return n_to_copy;
		}
	}
}
