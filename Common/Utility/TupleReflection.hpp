#pragma once

#include <concepts>
#include <type_traits>

namespace TupleReflection
{
	template <typename, typename>
	struct tuple_index {};

	template <typename... Types, typename T>
	struct tuple_index<std::tuple<T, Types...>, T> { static const std::size_t value = 0; };

	template <typename U, typename... Types, typename T>
	struct tuple_index<std::tuple<U, Types...>, T> { static const std::size_t value = 1 + tuple_index<std::tuple<Types...>, T>::value; };

	template<typename, typename>
	struct tuple_contains {};

	template<typename... Ts, typename T>
	struct tuple_contains<std::tuple<Ts...>, T> : std::disjunction<std::is_same<Ts, T>...> {};
}
