#pragma once

#include <memory>
#include "ClientState.hpp"

namespace Game
{
	class PlayerObject;
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


		std::unique_ptr<Game::PlayerObject> player;
	};
}
