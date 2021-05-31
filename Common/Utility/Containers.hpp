#pragma once

#include <algorithm>
#include <functional>
#include <ranges>

template<class T, typename Value>
auto Find( T& container, const Value& value ) -> decltype(T::iterator)
{
	return std::find( std::begin( container ), std::end( container ), value );
}

template<class T, typename Value>
auto Find( const T& container, const Value& value ) -> decltype(T::const_iterator)
{
	return std::find( std::begin( container ), std::end( container ), value );
}

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

template<std::ranges::range CONTAINER>
void Fill( CONTAINER& container, const std::ranges::range_value_t<CONTAINER>& value )
{
	std::fill( std::begin( container ), std::end( container ), value );
}

template<std::ranges::range CONTAINER>
bool AnyOf( const CONTAINER& container, std::predicate<std::ranges::range_value_t<CONTAINER>> auto pred )
{
	return std::any_of( std::begin( container ), std::end( container ), pred );
}
