#pragma once

#include <cinttypes>
#include <numeric>

namespace Server
{
	using ClientIndex_T = int;
	static const ClientIndex_T InvalidClientIndex = std::numeric_limits<ClientIndex_T>::max();

	using ClientId_T = uint64_t;
	static const ClientId_T InvalidClientId = std::numeric_limits<ClientId_T>::max();
}
