#include "ClientIdGenerator.hpp"

#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"

namespace Game::Networking
{
	ClientId GenerateClientId()
	{
		ClientId client_id{ 0 };
		yojimbo::random_bytes( (uint8_t*)&client_id, sizeof( client_id ) );

		return client_id;
	}
}
