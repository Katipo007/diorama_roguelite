#pragma once

#include "ClientServerCommon/Game/ClientSync/ClientSyncTypes.hpp"
#include "Common/DataTypes/Vec3.hpp"

namespace Game::Transform
{
	struct PositionComponent
	{
		std::array<float,3> position;

		void Serialise( typename auto& serialiser )
		{
			serialiser.container4b( position );
		}
	};
}
