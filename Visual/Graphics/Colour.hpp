#pragma once

#include <algorithm>
#include <cinttypes>
#include <functional>
#include <iostream>

#include "Common/Utility/Math/Mat4.hpp"

namespace Colour::Interpolation
{
	constexpr auto Linear = []( const uint8_t a, const uint8_t b, const float amount ) -> uint8_t
	{
		return static_cast<uint8_t>((b - a) * std::clamp( amount, 0.f, 1.f ) + a);
	};
}

//
// ColourRGBA
// packed 4-byte colour
//
struct ColourRGBA
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	inline constexpr ColourRGBA() noexcept : r( 0 ), g( 0 ), b( 0 ), a( 0 ) {}
	inline constexpr ColourRGBA( uint32_t aarrggbb ) noexcept : r( (aarrggbb >> 24u) & 0xFF ), g( (aarrggbb >> 16u) & 0xFF ), b( (aarrggbb >> 8u) & 0xFF ), a( aarrggbb & 0xFF ) {}
	inline constexpr ColourRGBA( uint8_t r, uint8_t g, uint8_t b ) noexcept : a( 255u ), r( r ), g( g ), b( b ) {}
	inline constexpr ColourRGBA( uint8_t r, uint8_t g, uint8_t b, uint8_t a ) noexcept : r( r ), g( g ), b( b ), a( a ) {}

	static inline ColourRGBA FromFloats( float r, float g, float b ) noexcept
	{
		return ColourRGBA(
			uint8_t( std::clamp( r, 0.f, 1.f ) * 255 ),
			uint8_t( std::clamp( g, 0.f, 1.f ) * 255 ),
			uint8_t( std::clamp( b, 0.f, 1.f ) * 255 ),
			255
		);
	}

	static inline ColourRGBA FromFloats( float a, float r, float g, float b ) noexcept
	{
		return ColourRGBA(
			uint8_t( std::clamp( r, 0.f, 1.f ) * 255 ),
			uint8_t( std::clamp( g, 0.f, 1.f ) * 255 ),
			uint8_t( std::clamp( b, 0.f, 1.f ) * 255 ),
			uint8_t( std::clamp( a, 0.f, 1.f ) * 255 )
		);
	}

	inline glm::vec4 AsFloatsRGBA() const { return { r / 255.f, g / 255.f, b / 255.f, a / 255.f }; }

	inline explicit operator int32_t() const { return (r << 24u) + (g << 16u) + (b << 8u) + a; }


	ColourRGBA Lerp( const ColourRGBA other, const float amount ) const
	{
		return {
			Colour::Interpolation::Linear( a, other.a, amount ),
			Colour::Interpolation::Linear( r, other.r, amount ),
			Colour::Interpolation::Linear( g, other.g, amount ),
			Colour::Interpolation::Linear( b, other.b, amount )
		};
	}

	ColourRGBA Interpolate( const ColourRGBA& other, const float amount, std::function< uint8_t( uint8_t, uint8_t, float ) > func = Colour::Interpolation::Linear )
	{
		return {
			func( a, other.a, amount ),
			func( r, other.r, amount ),
			func( g, other.g, amount ),
			func( b, other.b, amount )
		};
	}
};

static_assert(sizeof( ColourRGBA ) == sizeof( uint32_t ));
constexpr ColourRGBA operator"" _rgba( unsigned long long int v ) { return ColourRGBA( static_cast<uint32_t>(v) ); }

static_assert((0xFF000000_rgba).r == 255);
static_assert((0x00FF0000_rgba).g == 255);
static_assert((0x0000FF00_rgba).b == 255);
static_assert((0x000000FF_rgba).a == 255);
