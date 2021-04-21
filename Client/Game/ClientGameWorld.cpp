#include "ClientGameWorld.hpp"

#include "Visual/Renderer.hpp"
#include "Visual/SpriteBatcher.hpp"
#include "Visual/Device/Shader.hpp"
#include "Visual/Device/Texture.hpp"
#include "Visual/Device/VertexArray.hpp"

#include "Client/Game/PlayerObject.hpp"

namespace Game
{
	///
	/// ClientGameWorld
	/// 
	
	ClientGameWorld::ClientGameWorld( ResourceManager& _resources )
		: GameWorld()
		, resources( _resources )
	{
		sprite_renderer = std::make_unique<Visual::SpriteBatcher>();
		player = std::make_unique<PlayerObject>( _resources );
	}

	ClientGameWorld::~ClientGameWorld()
	{
		player.reset();
		sprite_renderer.reset();
	}

	void ClientGameWorld::OnFrame( const Timestep& timestep )
	{
		(void)timestep;
	}

	void ClientGameWorld::Render( std::shared_ptr<Visual::Camera> camera ) const
	{
		// SETUP
		Visual::Renderer::BeginScene( *camera );
		sprite_renderer->Begin( *camera );

		// ENVIRONMENT.
		
		// TILEMAP.
		
		// STATIC GEOMETRY.
		
		// GAME OBJECTS.
		RenderGameObjects();

		// PARTICLE SYSTEMS.

		// CLEANUP
		sprite_renderer->EndScene();
		Visual::Renderer::EndScene();
	}

	void ClientGameWorld::RenderGameObjects() const
	{
		/// Draw player specificly
		{
			ASSERT( player );
			sprite_renderer->DrawStandingImage( *player->GetImage(), { 0.f, 0.f, 0.f } );
		}
	}
}
