#pragma once

#include <concepts>
#include <type_traits>

template<typename T> requires(std::integral<T> || std::floating_point<T>)
struct Point2D
{
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

template<typename T> requires(std::integral<T> || std::floating_point<T>)
inline const Point2D<T> Point2D<T>::empty = { 0, 0 };
