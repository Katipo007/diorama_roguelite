#pragma once

#include "Common/Utility/Timestep.hpp"
#include "Common/Utility/NonCopyable.hpp"

namespace Game
{
	// Base type for ClientWorld and ServerWorld
	class GameWorld
		: NonCopyable
	{
	public:
		~GameWorld() = default;

		virtual void OnFrame( const Timestep& timestep ) = 0;

	protected:
		GameWorld() = default;
	};
}
