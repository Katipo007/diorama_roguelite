#pragma once

#include <concepts>
#include <memory>
#include <string_view>
#include <type_traits>

namespace Resources
{
	class Resource;
	class ResourceLoader;
	enum class AssetType;
}

namespace Resources::Concepts
{
	template<class T>
	concept Resource = requires
	{
		std::derived_from<T, ::Resources::Resource>;

		// haven't been able to get the `GetResourceType()` or `LoadResource()` return type requirements working :(
		//{ decltype( std::declval<T>() )::GetResourceType() } -> std::convertible_to<::Resources::AssetType>;
		//{ T::LoadResource( std::declval<::Resources::ResourceLoader&>() ) } -> std::convertible_to<std::shared_ptr<const T>>;
		//{ T::GetResourceType() } noexcept -> std::same_as<::Resources::AssetType>;
		//{ T::LoadResource( std::declval<ResourceLoader>() ) } -> std::convertible_to<std::shared_ptr<const T>>;
	};
}
