#pragma once

#include <type_traits>

template<typename T>
struct Point2D
{
	static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value, "Must be an integral or floating point type!");

	T x, y;

	constexpr Point2D() : x( 0 ), y( 0 ) {}
	constexpr Point2D( T x, T y ) : x( x ), y( y ) {}
	constexpr Point2D( const Point2D& other ) : x( other.x ), y( other.y ) {}
	constexpr Point2D& operator= ( const Point2D& rhs ) { this->x = rhs.x; this->y = rhs.y; return *this; }

	static const Point2D empty;

	constexpr Point2D operator-() const { return { -x, -y }; }

	constexpr bool operator==( const Point2D& rhs ) const { return (this->x == rhs.x) && (this->y == rhs.y); }
	constexpr bool operator!=( const Point2D& rhs ) const { return !(*this == rhs); }
};

template<typename T>
inline const Point2D<T> Point2D<T>::empty = { 0, 0 };
