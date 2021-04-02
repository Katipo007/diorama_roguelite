#include "PlayerObject.hpp"

#include "ClientGame.hpp"
#include "Visual/Resources/ImageResourceManager.hpp"

#include "Visual/Device/RendererCommand.hpp"
#include "Visual/Device/RendererAPI.hpp"
#include "Visual/Device/Shader.hpp"
#include "Visual/Device/Texture.hpp"
#include "Visual/Device/VertexArray.hpp"

#include "Visual/WorldSpace.hpp"

namespace Game
{
	PlayerObject::PlayerObject()
	{
		image = Game::GetClientGame().GetResourceImageManager().GetImageById( "2DArt/NPCs/OldWizard" );
		ASSERT( image );
	}

	PlayerObject::~PlayerObject()
	{
	}
}
