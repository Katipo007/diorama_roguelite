#pragma once

#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"

#define DEFINE_MESSAGE( name, members, methods, serialise ) \
	struct name \
		: public yojimbo::Message \
	{ \
		static constexpr std::string_view GetName() { return #name; } \
		\
		##members \
		\
		##methods \
		\
		template<typename STREAM> bool Serialize( STREAM& stream ) { (void)stream; ##serialise return true; } \
		\
		YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS() \
	};

namespace Networking::ClientServer
{	
	#include "Messages.def"
}

#undef DEFINE_MESSAGE