#pragma once

#include <memory>
#include "ClientState.hpp"

namespace Visual
{
	class Camera;
	class SphericalCamera;
}

namespace Game
{
	class PlayerObject;
	class ClientGameWorld;
}

namespace States
{
	class InGameState
		: public ClientState
	{
	public:
		InGameState();
		virtual ~InGameState();

		ClientStates GetStaticType() const { return ClientStates::InGame; };

	protected:
		virtual std::string_view GetName() const override { return "InGameState"; }

		virtual void OnAttached() override;
		virtual void OnDetached() override;

		virtual void OnEnabled() override;
		virtual void OnDisabled() override;

		virtual void OnFrame( const Timestep& ) override;
		virtual void OnDearImGuiRender() override;

		void Render() const;


		std::shared_ptr<Visual::SphericalCamera> main_camera;
		std::unique_ptr<Game::ClientGameWorld> gameworld; // TODO: refactor into client session
	};
}
