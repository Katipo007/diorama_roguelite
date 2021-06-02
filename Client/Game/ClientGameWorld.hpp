#pragma once

#include "ClientServerCommon/Game/GameWorld.hpp"

class ResourceManager;

namespace API { class VideoAPI; }

namespace Visual
{
	class Camera;
	class Renderer;
	class SpriteBatcher;
}

namespace Game
{
	class ClientGameWorld final
		: public GameWorld
	{
	public:
		ClientGameWorld( ::API::VideoAPI& video, ResourceManager& resources );
		~ClientGameWorld();

		void OnFixedUpdate( const PreciseTimestep& ) override;
		void OnVariableUpdate( const PreciseTimestep& ) override;

		// Render the world from a given camera's perspective
		void Render( std::shared_ptr<Visual::Camera> camera ) const;

	protected:
		void RenderGameObjects() const;

	protected:
		::API::VideoAPI& video;
		ResourceManager& resources;

		std::unique_ptr<Visual::Renderer> renderer;
		std::unique_ptr<Visual::SpriteBatcher> sprite_renderer;
	};
}
