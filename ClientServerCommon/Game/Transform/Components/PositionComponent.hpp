#pragma once

#include "ClientServerCommon/Game/ClientSync/ClientSyncTypes.hpp"
#include "Common/DataTypes/Vec3.hpp"

namespace Game::Transform
{
	struct PositionComponent
	{
		glm::vec3 position;

		void Serialise( typename auto& serialiser )
		{
			serialiser.value4b( position.x );
			serialiser.value4b( position.y );
			serialiser.value4b( position.z );
		}
	};
}
