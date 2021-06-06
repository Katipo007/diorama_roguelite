#pragma once

#include <concepts>

namespace Game
{
	template<class T>
	concept Component = requires
	{
		true;
	};
}
