#pragma once

#include "ClientServerCommon/Vendor/Wrappers/Networking.hpp"

namespace Networking
{
	namespace ChannelType
	{
		enum Type : unsigned
		{
			Reliable,
			Unreliable,

			NumChannelTypes,
		};
	}

	struct ClientServerGameConnectionConfig
		: public yojimbo::ClientServerConfig
	{
		ClientServerGameConnectionConfig();

		static const uint8_t DefaultPrivateKey[yojimbo::KeyBytes];
	};
}
