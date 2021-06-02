#include "ClientGameWorld.hpp"

#include "Common/Core/API/VideoAPI.hpp"
#include "Visual/Renderer.hpp"
#include "Visual/SpriteBatcher.hpp"
#include "Visual/Graphics/Shader.hpp"
#include "Visual/Graphics/Texture.hpp"
#include "Visual/Graphics/VertexArray.hpp"

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
	}

	ClientGameWorld::~ClientGameWorld()
	{
		sprite_renderer.reset();
	}

	void ClientGameWorld::OnFixedUpdate( const PreciseTimestep& timestep )
	{
		GameWorld::OnFixedUpdate( timestep );
	}

	void ClientGameWorld::OnVariableUpdate( const PreciseTimestep& timestep )
	{
		GameWorld::OnVariableUpdate( timestep );
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
		// TODO: call the GameEntityRenderer system
	}
}
