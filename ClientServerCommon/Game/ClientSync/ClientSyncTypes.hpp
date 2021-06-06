#pragma once

#include "ClientServerCommon/ecs.hpp"
#include "ClientServerCommon/Game/Concepts.hpp"
#include "Common/DataTypes/Bytes.hpp"

#include <concepts>
#include <sstream>
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

	using WriterS = bitsery::Serializer<Writer>;
	using ReaderS = bitsery::Serializer<Reader>;

	template<typename T>
	concept Serialiser = std::same_as<T, WriterS> || std::same_as<T, ReaderS>;

	template<typename T>
	concept SerialisableComponent = requires( T t )
	{
		Component<T>;
		{ t.Serialise( std::declval<WriterS&>() ) } -> std::same_as<void>;
		{ t.Serialise( std::declval<ReaderS&>() ) } -> std::same_as<void>;
	};
}
