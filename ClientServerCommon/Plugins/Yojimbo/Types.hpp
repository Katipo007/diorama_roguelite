#pragma once

#include <array>
#include <cinttypes>
#include <concepts>

namespace yojimbo { class Message; }

namespace YojimboPlugin
{
	using MessageType_T = int;
	using ClientIndex_T = int;
	using ClientId_T = uint64_t;
	using Key_T = std::array<uint8_t, 32>;
	using ChannelIndex_T = int;

	namespace Concepts
	{
		template<class T>
		concept Message = requires { std::is_base_of<::yojimbo::Message, T>::value; };

		template<class T>
		concept MessageInitialiser = requires( T x ) {
			std::is_base_of<::yojimbo::Message, T>::value;
			std::invocable<T&>;
		};
	}
}
