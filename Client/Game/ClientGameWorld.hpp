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
	class PlayerObject;

	class ClientGameWorld
		: public GameWorld
	{
	public:
		ClientGameWorld( ::API::VideoAPI& video, ResourceManager& resources );
		~ClientGameWorld();

		PlayerObject* GetPlayerObject() { return player.get(); }
		const PlayerObject* GetPlayerObject() const { return player.get(); }


		virtual void OnFrame( const Timestep& timestep ) override;
		// Render the world from a given camera's perspective
		void Render( std::shared_ptr<Visual::Camera> camera ) const;

	protected:
		void RenderGameObjects() const;

	protected:
		::API::VideoAPI& video;
		ResourceManager& resources;

		std::unique_ptr<PlayerObject> player;
		std::unique_ptr<Visual::Renderer> renderer;
		std::unique_ptr<Visual::SpriteBatcher> sprite_renderer;
	};
}
