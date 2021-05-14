#pragma once

#include "ClientServerCommon/Plugins/Yojimbo/Message.hpp"

#define BASIC_MESSAGE( message_name ) struct message_name : public ::YojimboPlugin::Message { \
	static constexpr std::string_view GetName() noexcept { return #message_name; } \
	template<typename STREAM> bool Serialize( STREAM& ) { return true; } \
	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS(); \
};


/////////////////////////////////////////////////////////////

namespace Game::Networking
{

	BASIC_MESSAGE( Dummy );

}

/////////////////////////////////////////////////////////////

#undef BASIC_MESSAGE
