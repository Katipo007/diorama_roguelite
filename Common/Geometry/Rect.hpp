#pragma once

#include <type_traits>

#include "Common/Geometry/Point2D.hpp"

template<typename T>
class Rect
{
	static_assert( std::is_integral<T>::value || std::is_floating_point<T>::value, "Type must be a floating point or integral type" );
public:
	static const Rect empty;

public:
	constexpr Rect() : x( 0 ), y( 0 ), w( 0 ), h( 0 ) {}
	constexpr Rect( T x1, T y1, T x2, T y2 ) : x( x1 ), y( y1 ), w( x2 - x1 ), h( y2 - y1 ) {}
	constexpr Rect( const Point2D<T>& min, const Point2D<T>& max ) : Rect( min.x, min.y, max.x, max.y ) { ASSERT( IsValid() ); }

	constexpr T GetLeft() const { return x; }
	constexpr T GetTop() const { return y; }
	constexpr T GetRight() const { return x + w; }
	constexpr T GetBottom() const { return y + h; }

	_NODISCARD constexpr Point2D<T> GetPosition() const { return { x, y }; }
	constexpr T GetX() const { return x; }
	constexpr T GetY() const { return y; }
	constexpr T GetWidth() const { return w; }
	constexpr T GetHeight() const { return h; }

	// "Valid" rectangles are those with positive sizes
	constexpr bool IsValid() const { return w >= 0 && h >= 0; }
	// true if width or height > 0
	constexpr bool HasDimension() const { return w > 0 && h > 0; }

	Rect& Inset( T value ) { x += value; y += value; w -= value * 2; h -= value * 2; return *this; }
	Rect& Inset( T dx, T dy ) { this->x += dx; this->y += dy; this->w -= dx * 2, this->y -= dy * 2; return *this; }

	Rect& Move( T dx, T dy ) { this->x += dx; this->y += dy; return *this; }
	Rect& Move( const Point2D<T>& p ) { return Move( p.x, p.y ); }

	Rect& Clip( const Rect& mask )
	{
		const auto clipped_x2 = std::min( GetRight(), mask.GetRight() );
		const auto clipped_y2 = std::min( GetBottom(), mask.GetBottom() );

		x = std::max( x, mask.x );
		y = std::max( y, mask.y );
		w = clipped_x2 - x;
		h = clipped_y2 - y;

		return *this;
	}

	constexpr bool Contains( const Point2D<T>& p ) const { return HasDimension() && (p.x >= this->x && p.x < (this->x + this->w)) && (p.y >= this->y && p.y < (this->y + this->h)); }

	constexpr bool operator==( const Rect& rhs ) const { return (this->x == rhs.x) && (this->y == rhs.y) && (this->w == rhs.w) && (this->h == rhs.h); }
	constexpr bool operator!=( const Rect& rhs ) const { return !(*this == rhs); }

	T x, y;
	T w, h;
};

template<typename T>
inline const Rect<T> Rect<T>::empty = { 0, 0, 0, 0 };
