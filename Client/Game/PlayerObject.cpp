#include "PlayerObject.hpp"

#include "ClientGame.hpp"
#include "Common/Core/ResourceManager.hpp"
#include "Visual/Resources/ImageResourceManager.hpp"

#include "Visual/Device/RendererCommand.hpp"
#include "Visual/Device/RendererAPI.hpp"
#include "Visual/Device/Shader.hpp"
#include "Visual/Device/Texture.hpp"
#include "Visual/Device/VertexArray.hpp"

#include "Visual/WorldSpace.hpp"

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
