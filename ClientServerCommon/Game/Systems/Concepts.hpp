#pragma once

#include <concepts>

namespace Game::Systems
{
	class BaseSystem;
	enum class SystemType : uint8_t;
	
	template<typename T>
	concept System = requires
	{
		std::derived_from<T, BaseSystem>;
		std::is_same< std::string_view, decltype(std::declval<T&>().GetName()) >::value; // Must provide a `static constexpr std::string_view GetName() noexcept` method
		{ T::GetStaticType() } -> std::same_as<SystemType>; // Must provide a static system type getter
	};
}
