#pragma once

#include "ClientServerCommon/ecs.hpp"

#include <string_view>

namespace Game::Name::Helpers
{
	void SetName( const ecs::EntityHandle entity, std::string_view name );
	std::string_view GetName( const ecs::EntityConstHandle entity );
}
