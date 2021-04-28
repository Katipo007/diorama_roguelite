#include "PlayerObject.hpp"

#include "ClientGame.hpp"
#include "Common/Core/ResourceManager.hpp"
#include "Visual/Resources/SpriteSheet.hpp"

namespace Game
{
	PlayerObject::PlayerObject( ResourceManager& _resources )
	{
		sprite = _resources.Get<Graphics::Sprite>( "2DArt/NPCs/OldWizard" );
		ASSERT( sprite );
	}

	PlayerObject::~PlayerObject()
	{
	}
}
