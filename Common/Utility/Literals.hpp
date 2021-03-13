#pragma once

#include <cinttypes>

constexpr uint8_t operator"" _u8( unsigned long long int v )
{
	return v;
}