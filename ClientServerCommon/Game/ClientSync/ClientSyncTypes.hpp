#pragma once

#include <sstream>

#include "ClientServerCommon/ecs.hpp"
#include "ClientServerCommon/Game/Concepts.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"

namespace Game::ClientSync
{
	using EntitySyncId = uint64_t;
	using ComponentTypeId = entt::id_type;

	using ReadStream = yojimbo::ReadStream;
	using WriteStream = yojimbo::WriteStream;

	template<typename T>
	concept SerialisableComponent = requires(T t, const T c)
	{
		Component<T>;
		{ c.Serialise( std::declval<WriteStream&> ) } -> std::same_as<bool>;
		{ t.Serialise( std::declval<ReadStream&> ) } -> std::same_as<bool>;
	};
}
