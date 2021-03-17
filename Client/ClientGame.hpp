#pragma once

#include <memory>

#include "Common/Utility/Timestep.hpp"

int main( int, char** );

namespace States
{
	class ClientStateManager;
}

namespace Game
{
	class ClientGame;

	ClientGame& GetClientGame();

	class ClientGame final
	{
		friend int ::main( int, char** );

	public:
		~ClientGame();

	protected:
		ClientGame(); // for entry point to call

		void OnFrame( const Timestep& ts ); // for entry point to call

		std::unique_ptr<States::ClientStateManager> state_manager;
	};
}
