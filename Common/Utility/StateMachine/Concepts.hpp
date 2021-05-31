#pragma once

#include <concepts>
#include <type_traits>

namespace fsm::Concepts
{
	template<class T>
	concept State = requires
	{
		std::movable<T>;
		std::is_trivially_move_constructible<T>::value; // All state types must be moveable
	};

	template<class T>
	concept Event = requires
	{
		std::movable<T>;
		std::is_trivially_move_constructible<T>::value;
	};

}

namespace fsm
{
	template<Concepts::State... _States>
	class States;

	template<Concepts::Event... _Events>
	class Events;

	template<class States, class Events>
	class Machine;
}

namespace fsm::Concepts
{
	template<class T>
	concept Action = requires( T t )
	{
		std::copyable<T>;
		std::movable<T>;
		std::is_trivially_move_constructible<T>::value;
		{ t.Execute( std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) } -> std::same_as<void>;
	};
}