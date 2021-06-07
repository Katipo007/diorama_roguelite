#pragma once

#include "ClientServerCommon/Game/ClientSync/ClientSyncTypes.hpp"
#include <string>

#include "Common/DataTypes/Colour.hpp"
namespace Game::Sprite
{
	struct CommonSpriteComponent
	{
		std::string id;
		ColourRGBA blend;

		void Serialise( typename auto& serialiser )
		{
			serialiser.text1b( id, 128 );
			serialiser.value4b( (uint32_t&)blend ); static_assert(sizeof( blend ) == 4);
		}
	};
}
