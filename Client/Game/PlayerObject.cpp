#include "PlayerObject.hpp"

#include "ClientGame.hpp"
#include "Common/Core/ResourceManager.hpp"

namespace Game
{
	PlayerObject::PlayerObject( ResourceManager& _resources )
	{
		(void)_resources;
		//image = _resources.Get<Resources::Image>( "2DArt/NPCs/OldWizard" );
		ASSERT( image );
	}

	PlayerObject::~PlayerObject()
	{
	}
}
