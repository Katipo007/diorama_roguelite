#pragma once

#include "Common/Utility/Entity/EnttHeader.hpp"

namespace ecs
{
	using Registry = entt::registry;
	using Entity = Registry::entity_type;
	using EntityHandle = entt::basic_handle<Entity>;
	using EntityConstHandle = entt::basic_handle<const Entity>;

	static const Entity NullEntity = entt::null;
}
