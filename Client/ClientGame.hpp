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
		ClientGame(); // private constructor to only allow int main() to create us.

		void OnFrame( const Timestep& ts );

		std::unique_ptr<States::ClientStateManager> state_manager;
	};
}
