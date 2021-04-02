#pragma once

#include "ClientServerCommon/Game/GameWorld.hpp"

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
		ClientGameWorld();
		~ClientGameWorld();

		PlayerObject* GetPlayerObject() { return player.get(); }
		const PlayerObject* GetPlayerObject() const { return player.get(); }


		virtual void OnFrame( const Timestep& timestep ) override;
		// Render the world from a given camera's perspective
		void Render( std::shared_ptr<Visual::Camera> camera ) const;

	protected:
		void RenderGameObjects() const;

	protected:
		std::unique_ptr<PlayerObject> player;
		std::unique_ptr<Visual::SpriteBatcher> sprite_renderer;
	};
}
