#pragma once

#include "ClientServerCommon/ecs.hpp"
#include "ClientServerCommon/Game/Concepts.hpp"

#include <concepts>
#include <unordered_map>
#include <bitsery/include/bitsery/bitsery.h>
#include <bitsery/include/bitsery/adapter/buffer.h>
#include <bitsery/include/bitsery/traits/array.h>
#include <bitsery/include/bitsery/traits/string.h>
#include <bitsery/include/bitsery/traits/vector.h>

namespace Game::ClientSync
{
	using EntitySyncId = uint64_t;
	using ComponentTypeId = entt::id_type;

	using Buffer_T = std::vector<uint8_t>;
	using Writer = bitsery::OutputBufferAdapter<Buffer_T>;
	using Reader = bitsery::InputBufferAdapter<Buffer_T>;

	using Serialiser = bitsery::Serializer<Writer>;
	using Deserialiser = bitsery::Deserializer<Reader>;

	template<typename T>
	concept SerialisableComponent = requires(T t)
	{
		Component<T>;
		{ t.Serialise( std::declval<Serialiser&>() ) } -> std::same_as<void>;
		{ t.Serialise( std::declval<Deserialiser&>() ) } -> std::same_as<void>;
	};

	using SyncedEntityMap_T = std::unordered_map<EntitySyncId, ecs::EntityHandle>;
}
