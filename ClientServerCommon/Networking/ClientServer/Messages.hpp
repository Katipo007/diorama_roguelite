#pragma once

#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"

namespace Networking::ClientServer
{
#undef BASIC_MESSAGE
#define BASIC_MESSAGE( name ) struct name : public yojimbo::Message { static constexpr std::string_view GetName() { return #name; } template<typename STREAM> bool Serialize( STREAM& ) { return true; } YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS(); }

	///
	/// DEFINE MESSAGES HERE
	///
	
	BASIC_MESSAGE( Dummy );






	/// 
	/// STOP DEFINING MESSAGES
	/// 

#undef BASIC_MESSAGE
}
