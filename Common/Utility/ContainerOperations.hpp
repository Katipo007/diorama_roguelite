#pragma once

#include <algorithm>
#include <functional>
#include <ranges>

template<std::ranges::range CONTAINER>
auto Find( CONTAINER& container, const std::ranges::range_value_t<CONTAINER>& value ) -> std::ranges::iterator_t<CONTAINER>
{
	return std::find( std::begin( container ), std::end( container ), value );
}

template<std::ranges::range CONTAINER>
auto Find( const CONTAINER& container, const std::ranges::range_value_t<const CONTAINER>& value ) -> std::ranges::iterator_t<const CONTAINER>
{
	return std::find( std::begin( container ), std::end( container ), value );
}

template<std::ranges::range CONTAINER>
auto FindIf( CONTAINER& container, std::predicate<std::ranges::range_value_t<const CONTAINER>> auto pred ) -> std::ranges::iterator_t<CONTAINER>
{
	return std::find_if( std::begin( container ), std::end( container ), pred );
}

template<std::ranges::range CONTAINER>
auto FindIf( const CONTAINER& container, std::predicate<std::ranges::range_value_t<const CONTAINER>> auto pred ) -> std::ranges::iterator_t<const CONTAINER>
{
	return std::find_if( std::begin( container ), std::end( container ), pred );
}

template<std::ranges::range CONTAINER>
void ForEach( CONTAINER& container, std::invocable<std::ranges::range_value_t<CONTAIENR>> auto func )
{
	std::for_each( std::begin( container ), std::end( container ), func );
}

template<std::ranges::range CONTAINER>
void ForEach( const CONTAINER& container, std::invocable<std::ranges::range_value_t<const CONTAIENR>> auto func )
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
