#pragma once

#include <type_traits>

namespace yojimbo { class Message; }

namespace YojimboPlugin::Concepts
{
	template<class T>
	concept Message = requires
	{
		std::is_base_of<::yojimbo::Message, T>::value; // A message must derive from ::yojimbo::Message
		std::is_default_constructible<T>::value; // A message must be default constructable
		std::is_same< std::string_view, decltype(std::declval<T&>().GetName()) >::value; // A message must provide a `static constexpr std::string_view GetName() noexcept` method
	};

	template<class T>
	concept MessageInitialiser = requires(T x) {
		std::is_base_of<::yojimbo::Message, T>::value;
		std::invocable<T&>;
	};
}
