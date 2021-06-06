#pragma once

#include "ClientServerCommon/ecs.hpp"

#include <string>

namespace Game::Name::Helpers
{
	void SetName( const ecs::EntityHandle entity, std::string_view name );
}
