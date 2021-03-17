#include "ClientGameWorld.hpp"

#include "Visual/Renderer.hpp"
#include "Visual/Device/Shader.hpp"
#include "Visual/Device/Texture.hpp"
#include "Visual/Device/VertexArray.hpp"

#include "Client/Game/PlayerObject.hpp"

namespace Game
{
	///
	/// ClientGameWorld
	/// 
	
	ClientGameWorld::ClientGameWorld()
		: GameWorld()
	{
		player = std::make_unique<PlayerObject>();
	}

	ClientGameWorld::~ClientGameWorld()
	{
		player.reset();
	}

	void ClientGameWorld::OnFrame( const Timestep& timestep )
	{
		(void)timestep;
	}

	void ClientGameWorld::Render( std::shared_ptr<Visual::Camera> camera ) const
	{
		// SETUP
		Visual::Renderer::BeginScene( *camera );

		// ENVIRONMENT.
		
		// TILEMAP.
		
		// STATIC GEOMETRY.
		
		// GAME OBJECTS.
		RenderGameObjects();

		// PARTICLE SYSTEMS.

		// CLEANUP
		Visual::Renderer::EndScene();
	}

	void ClientGameWorld::RenderGameObjects() const
	{
		/// Draw player specificly
		{
			ASSERT( player );
			const auto& player_texture = player->GetTexture();

			player_texture->Bind( 0 );
			Visual::Renderer::Submit( player->GetShader(), player->GetModel() );
		}
	}
}
