#pragma once

#include "ClientServerCommon/Game/GameWorld.hpp"

class ResourceManager;

namespace Visual
{
	class Camera;
	class SpriteBatcher;
}

namespace Game
{
	class PlayerObject;

	class ClientGameWorld
		: public GameWorld
	{
	public:
		ClientGameWorld( ResourceManager& resources );
		~ClientGameWorld();

		PlayerObject* GetPlayerObject() { return player.get(); }
		const PlayerObject* GetPlayerObject() const { return player.get(); }


		virtual void OnFrame( const Timestep& timestep ) override;
		// Render the world from a given camera's perspective
		void Render( std::shared_ptr<Visual::Camera> camera ) const;

	protected:
		void RenderGameObjects() const;

	protected:
		ResourceManager& resources;

		std::unique_ptr<PlayerObject> player;
		std::unique_ptr<Visual::SpriteBatcher> sprite_renderer;
	};
}
