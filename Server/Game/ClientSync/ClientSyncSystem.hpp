#pragma once

#include "ClientServerCommon/ecs.hpp"

namespace Game::ClientSync
{
	void AttachHandlers( ecs::Registry& registry );
	void System( ecs::Registry& registry );
	void DetatchHandlers( ecs::Registry& registry );
}
