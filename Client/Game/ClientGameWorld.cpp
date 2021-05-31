#include "ClientGameWorld.hpp"

#include "Common/Core/API/VideoAPI.hpp"
#include "Visual/Renderer.hpp"
#include "Visual/SpriteBatcher.hpp"
#include "Visual/Graphics/Shader.hpp"
#include "Visual/Graphics/Texture.hpp"
#include "Visual/Graphics/VertexArray.hpp"

#include "Client/Game/PlayerObject.hpp"

namespace Game
{
	///
	/// ClientGameWorld
	/// 
	
	ClientGameWorld::ClientGameWorld( ::API::VideoAPI& video_, ResourceManager& resources_ )
		: GameWorld()
		, video( video_ )
		, resources( resources_ )
	{
		renderer = std::make_unique<Visual::Renderer>( video );
		sprite_renderer = std::make_unique<Visual::SpriteBatcher>( video );
		player = std::make_unique<PlayerObject>( resources );
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
		renderer->BeginScene( *camera );
		sprite_renderer->Begin( *camera );

		// ENVIRONMENT.
		
		// TILEMAP.
		
		// STATIC GEOMETRY.
		
		// GAME OBJECTS.
		RenderGameObjects();

		// PARTICLE SYSTEMS.

		// CLEANUP
		sprite_renderer->EndScene();
		renderer->EndScene();
	}

	void ClientGameWorld::RenderGameObjects() const
	{
		/// Draw player specificly
		{
			ASSERT( player );
			sprite_renderer->DrawStandingSprite( player->GetSprite(), player->GetPosition() );
		}
	}
}
