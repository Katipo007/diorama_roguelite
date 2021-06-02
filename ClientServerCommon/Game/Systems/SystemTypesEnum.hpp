#pragma once

namespace Game::Systems
{
	enum class SystemType : uint8_t
	{
		Unknown = 0,

		GameEntityManager,	//< This system manages the lifecycle of game entities within the register
		NetworkManager,		//< This system handles synchronisation of the entity register between server & clients
	};
}
