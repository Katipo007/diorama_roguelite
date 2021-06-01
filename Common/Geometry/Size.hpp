#pragma once

#include <type_traits>
#include <limits>

#include "Point2D.hpp"
#include "Common/Core/Assert.hpp"

template<typename T> requires(std::integral<T> || std::floating_point<T>)
struct Size
{
	// TODO: Look at using SafeInt for integral types

public:
	T width, height;

	constexpr Size() : width( 0 ), height( 0 ) {}
	constexpr Size( T width, T height ) : width( width ), height( height ) {}

	operator Point2D<T>() const { return { width, height }; }

	inline Size Inflate( const Size& o ) const { return Size( width + o.width, height + o.height ); }
	constexpr Size operator+( const Size& o ) const { return Size( width + o.width, height + o.height ); }
	constexpr Size operator-( const Size& o ) const { return Size( width - o.width, height - o.height ); }

	constexpr Size operator*( const Size& o ) const { return Size( width * o.width, height * o.height ); }
	constexpr Size operator*( float v ) const { return Size( static_cast<T>(width * v), static_cast<T>(height * v) ); }

	constexpr Size operator/( const Size& o ) const { return Size( width / o.width, height / o.height ); }
	constexpr Size operator/( float v ) const { return Size( static_cast<T>(width / v), static_cast<T>(height / v) ); }

	Size& operator+=( const Size& o ) { width += o.width; height += o.height; return *this; }
	Size& operator-=( const Size& o ) { width -= o.width; height -= o.height; return *this; }

	constexpr bool operator==( const Size& other ) const { return (width == other.width) && (height == other.height); }
	constexpr bool operator!=( const Size& other ) const { return !(*this == other); }

	static const Size empty;
};

template<typename T> requires(std::integral<T> || std::floating_point<T>)
inline const Size<T> Size<T>::empty = { 0, 0 };
