#pragma once

#include <algorithm>
#include <functional>

template<class T, typename Pred>
auto FindIf( T& container, const Pred& pred ) -> decltype(T::iterator)
{
	return std::find_if( std::begin( container ), std::end( container ), pred );
}

template<class T, typename Pred>
auto FindIf( const T& container, const Pred& pred ) -> decltype(T::const_iterator)
{
	return std::find_if( std::begin( container ), std::end( container ), pred );
}

template<class T, typename Func>
void ForEach( T& container, const Func& func )
{
	std::for_each( std::begin( container ), std::end( container ), func );
}

template<class T, typename Func>
void ForEach( const T& container, const Func& func )
{
	std::for_each( std::begin( container ), std::end( container ), func );
}
