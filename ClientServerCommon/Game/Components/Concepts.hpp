#pragma once

namespace Game::Components
{
	template<class T>
	concept Component = requires
	{
		true;
	};
}
